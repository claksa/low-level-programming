
#ifndef LLP_DATABASE_H
#define LLP_DATABASE_H

#include <cstddef>
#include <utility>
#include "scheme.h"
#include "region.h"

// TODO errors handling!

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
        cout << "---" << endl;
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

};

#endif //LLP_DATABASE_H
