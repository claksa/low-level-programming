
#ifndef LLP_DATABASE_H
#define LLP_DATABASE_H

#include <cstddef>
#include <utility>
#include "scheme.h"
#include "region.h"

// TODO errors handling!
// TODO чекать что не ушли за "пределы" документа
// TODO change field visibility

class database_info {
public:
    int num_cols = DEFAULT_COL_NUM;
    int num_docs = DEFAULT_DOCS_NUM;
    string name = DATABASE;

    database_info() = default;
    database_info(int num_cols, int num_docs, string  name) : num_cols(num_cols),
                                                              num_docs(num_docs),
                                                              name(std::move(name))
    {}
};

class database {
    database_info meta;
//    vector<region> regions;

public:
    // removed_id <-> its offset
    map<long, long> removed_collections;
    map<long, long> existed_collections;
    long end_of_last_collection = DB_OFFSET;
    static inline long col_docs_size = NODE_INFO_SIZE+NODE_TREE_INFO_SIZE+COLLECTION_HEADER_SIZE;
    explicit database(database_info meta) : meta(std::move(meta)) {}

    void open(fstream &filestream) {
        filestream.open(meta.name, ios::binary|ios::out|ios::in);
        write_db_info(filestream);
        cout << *this;
    }

    static void close(fstream &filestream) {
        filestream.close();
    }

    friend ostream& write(ostream& out, database& db) {
        out.write(reinterpret_cast<const char*>(&db.meta),sizeof(db.meta));
        return out;
    }

    friend ostream& operator<<(ostream& os, const database& db) {
        cout << "---META_INF---" << endl;
        cout << "database name (filename): " << db.meta.name << endl;
        cout << "maximum number of collections: " << db.meta.num_cols << endl;
        cout << "maximum number of documents in each collection: " << db.meta.num_docs << endl;
        return os;
    }

    void write_db_info(ostream& out) {
        write(out, *this);
    }

    static void insert_collection(fstream &filestream, collection &col, long offset) {
        long start_documents = offset + region::get_collection_header_offset();
        long end_documents = start_documents + region::get_docs_offset();
        long end_collection = end_documents + region::get_doc_tree_offset();
        col.set_header(collection_meta_info (offset,
                                             start_documents,
                                             end_documents,
                                             end_collection));

        col.write_collection(filestream);
        cout << "pointer after write collection: " << filestream.tellp() << endl;
    }

    // TODO refactor
    void create_collection(fstream &filestream, collection &col) {
        if (!removed_collections.empty()) {
            long offset = removed_collections.begin()->second;
            insert_collection(filestream, col, offset);
            removed_collections.erase(removed_collections.begin());
            existed_collections.emplace(col.header.collection_id, offset);
            return;
        }
        long col_offset = end_of_last_collection;
        region reg;

        // TODO make it abstract
        // allocate region for collection header
        reg.allocate_collection_region(filestream, col_offset);
        long start_documents = (long) filestream.tellp();

        // allocate region for possible documents
        reg.allocate_doc_region(filestream, start_documents);
        long end_documents = (long) filestream.tellp();

        // allocate region for document tree
        reg.allocate_doc_tree_region(filestream, end_documents);
        long end_collection = (long) filestream.tellp();
        this->end_of_last_collection = end_collection;

        col.set_header(collection_meta_info (col_offset,
                                             start_documents,
                                             end_documents,
                                             end_collection));

        col.write_collection(filestream);
        existed_collections.emplace(col.header.collection_id, col_offset);
    }

   void read_collection_header(fstream &file, collection &col) {
        long id = col.header.collection_id;
       if (auto search = removed_collections.find(id); search != removed_collections.end()) {
           cout << "FOUND AS REMOVED: " << search->first << endl;
           return;
       } else cout << "not found as removed collection " << endl;
        long offset = existed_collections.at(id);
        col.read_collection(file, offset);
        cout << "read collection header: " << endl;
        cout << col;
    }

    void delete_collection(fstream &file, long &id) {
        auto* col = new collection();
        col->header.collection_id = id;
        read_collection_header(file, *col);
        long offset = col->header.offset_from_beg;
        if (col->header.is_empty) {
            region reg;
            // == fill it zeroes
            reg.allocate_collection_region(file, offset);
            removed_collections.emplace(id, offset);
            existed_collections.erase(id);
        } else cout << "ERROR: can't delete collection, it's not empty" << endl;
    }

    void create_node(fstream& file, add_node_query* query) {
        auto* col = new collection();
        col->header.collection_id = query->collection_id;
        cout << "IN CREATE NODE: " << endl;
        read_collection_header(file, *col);
        col->header.is_empty = false;

        property_field* field = col->sch.field;
        auto* property1 = new property(field[0].property_name,
                                       query->properties[0],
                                       static_cast<DataTypes>(field[0].data_type));
        auto* property2 = new property(field[1].property_name,
                                       query->properties[1],
                                       static_cast<DataTypes>(field[1].data_type));

        // пока так,хз что делать с свойствами ещё (так чтобы добавлял именно объект коллекции!)
        long property_offset = col->header.end_of_last_property;
        property1->add_property(file, property_offset);
        col->header.end_of_last_property = property2->add_property(file, property_offset + (long)sizeof(property));
        cout << "pointer after write properties(end_of_last_property): " << col->header.end_of_last_property << endl;
        auto* node = new doc_tree_info(query->parent_id,
                                       query->node_name,
                                       property_offset,
                                       query->children_size,
                                       2);
        col->header.end_of_last_node = col->insert_node(file, *node);
        cout << "pointer after write node info(end_of_last_node): " << col->header.end_of_last_node << endl;
        // update header
        col->write_collection(file);
    }

    void read_all_nodes(fstream& file, long collection_id) {
        auto* col = new collection();
        col->header.collection_id = collection_id;
        cout << "IN READ ALL: " << endl;
        read_collection_header(file, *col);
        if (col->header.is_empty) {
            cout << "no documents in collection " << endl;
            return;
        }
        col->read_node_tree(file);
    }

    void enumerate_nodes_by_filter(fstream& file, filter_query& query) {
        auto* col = new collection();
        col->header.collection_id = query.collection_id;
        cout << "IN READ operation: " << endl;
        read_collection_header(file, *col);
        if (col->header.is_empty) {
            cout << "no nodes to enumerate " << endl;
        }
        // TODO validate node_type from query
        col->enumerate_by_filter(file, query.filter);
    }

};

#endif //LLP_DATABASE_H
