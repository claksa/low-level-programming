
#ifndef LLP_DATABASE_H
#define LLP_DATABASE_H

#include <cstddef>
#include <utility>
#include "scheme.h"
#include "region.h"

// TODO errors handling!
// TODO change field visibility

class database_info {
public:
    string name = DATABASE;

    database_info() = default;
    database_info(int num_cols, int num_docs, string  name) : name(std::move(name))
    {}
};

class database {
    database_info meta;
//    vector<region> regions;

public:
    // removed_id <-> its offset
    map<long, long> removed_collections;
    map<long, long> existed_collections;
    long current_pointer = DB_OFFSET;
    static inline long col_docs_size = NODE_INFO_SIZE+NODE_TREE_INFO_SIZE+COLLECTION_HEADER_SIZE;
    explicit database(database_info meta) : meta(std::move(meta)) {}

    void open(fstream &filestream) {
        filestream.open(meta.name, ios::binary|ios::out|ios::in);
//        write_db_info(filestream);
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
        return os;
    }

    void write_db_info(ostream& out) {
        write(out, *this);
    }

    static void insert_collection(fstream &filestream, collection &col, long offset) {
        long start_documents = offset + region::get_collection_header_offset();
        long end_documents = start_documents + region::get_docs_offset();
        long end_collection = end_documents + region::get_doc_tree_offset();
        col.set_header(collection_meta_info (offset));

        col.write_collection(filestream);
        cout << "pointer after write collection: " << filestream.tellp() << endl;
    }

    void create_region_header(fstream &filestream, region_type type, long offset) {
        filestream.seekp(offset, ios_base::beg);
        cout << "pointer before create region header: " << filestream.tellp() << endl;
        region_header header = {.type=type, .is_removed = false};
        filestream.write(reinterpret_cast<char*>(&header), sizeof(header));
    }

    void read_region_header(fstream &filestream, region_header &header, long offset) {
        filestream.seekp(offset, ios::beg);
//        cout << "pointer before read region header: " << filestream.tellp() << endl;
        filestream.read(reinterpret_cast<char*>(&header), sizeof(header));
//        cout << "region type: " << static_cast<region_type>(header.type) << endl;
    }

    void create_collection(fstream &filestream, collection &col) {
        create_region_header(filestream, region_type::COLLECTION, (long) filestream.tellp());
        long col_offset = (long) filestream.tellp();
        cout << "offset before write collection: " << col_offset << endl;
        if (!removed_collections.empty()) {
            long offset = removed_collections.begin()->second;
            insert_collection(filestream, col, offset);
            removed_collections.erase(removed_collections.begin());
            existed_collections.emplace(col.header.collection_id, offset);
            return;
        }
        col.set_header(collection_meta_info (col_offset));
        col.write_collection(filestream);
        existed_collections.emplace(col.header.collection_id, col_offset);
        current_pointer = (long) filestream.tellp();
        cout << "pointer after created collection: " << current_pointer << endl;
    }

   void read_collection_header(fstream &file, collection &col) {
        long id = col.header.collection_id;
       if (auto search = removed_collections.find(id); search != removed_collections.end()) {
           cout << "FOUND AS REMOVED: " << search->first << endl;
           cout << "cannot read this collection" << endl;
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
            reg.allocate_collection_region(file, offset);
            removed_collections.emplace(id, offset);
            existed_collections.erase(id);
            cout << "removed collection" << endl;
        } else cout << "ERROR: can't delete collection, it's not empty" << endl;
    }

    void create_node(fstream& file, add_node_query* query) {
        auto* col = new collection();
        col->header.collection_id = query->collection_id;
        cout << "IN CREATE NODE: " << endl;
        read_collection_header(file, *col);
        col->header.is_empty = false;
        create_region_header(file, NODE, current_pointer);
        property_field* field = col->sch.field;
        current_pointer = current_pointer + (long)sizeof(region_header);
        file.seekp(current_pointer, ios_base::beg);
        cout << "pointer before write node tree info: " << file.tellp() << endl;
        auto* property1 = new property(field[0].property_name,
                                       query->properties[0],
                                       static_cast<DataTypes>(field[0].data_type));
        auto* property2 = new property(field[1].property_name,
                                       query->properties[1],
                                       static_cast<DataTypes>(field[1].data_type));
        auto* node = new doc_tree_info(query->collection_id,
                                       query->parent_id,
                                       query->node_name,
                                       query->children_size,
                                       2);
        col->insert_node(file, *node);
        cout << "pointer after write node: " << file.tellp() << endl;
        long property_offset = (long) file.tellp();
        property1->add_property(file, property_offset);
        property2->add_property(file, property_offset + (long)sizeof(property));
        current_pointer = (long) file.tellp();
        cout << "pointer after write properties: " << current_pointer << endl;
        // update header
        col->write_collection(file);
    }

    void read_all_docs_database(fstream& file) {
        cout << "IN READ ALL: " << endl;
        file.seekp(ios_base::beg);
        long pointer = (long) file.tellp();
        while(pointer != current_pointer) {
            region_header header;
            read_region_header(file, header, pointer);
            if (header.type == COLLECTION) {
                file.seekp(COLLECTION_HEADER_SIZE, ios_base::cur);
                pointer = (long) file.tellp();
            }
            if (header.type == NODE) {
                cout << "node doc tree: " << endl;
                auto* node = new doc_tree_info();
                node->read_node(file, (long)file.tellg());
                cout << *node;
                cout << "---" << endl;
                long start_property = (long) file.tellg();
                for (int i = 0; i < node->real_properties_size; i++) {
                    auto* p = new property();
                    p->read_property(file,(long) (start_property + i*sizeof(property)));
                    cout << *p;
                }
                pointer = (long) file.tellp();
                cout << endl;
            }
        }
    }

    void read_collection_documents(fstream& file, long collection_id) {
        auto* read_col = new collection();
        read_col->header.collection_id = collection_id;
        read_collection_header(file, *read_col);
        if (read_col->header.is_empty) {
            cout << "no nodes to enumerate " << endl;
        }
        cout << "IN READ COLLECTION: " << endl;
        file.seekp(ios_base::beg);
        long pointer = (long) file.tellp();
        while(pointer != current_pointer) {
            region_header header;
            read_region_header(file, header, pointer);
            if (header.type == COLLECTION) {
                file.seekp(COLLECTION_HEADER_SIZE, ios_base::cur);
                pointer = (long) file.tellp();
            }
            if (header.type == NODE) {
                auto* node = new doc_tree_info();
                node->read_node(file, (long)file.tellg());
                if (node->collection_id != collection_id) {
                    long properties_size = (long) (node->real_properties_size* sizeof(property));
                    file.seekp(properties_size, ios_base::cur);
                    pointer = (long) file.tellp();
                    continue;
                }
                cout << "node doc tree: " << endl;
                cout << *node;
                cout << "---" << endl;
                long start_property = (long) file.tellg();
                for (int i = 0; i < node->real_properties_size; i++) {
                    auto* p = new property();
                    p->read_property(file,(long) (start_property + i*sizeof(property)));
                    cout << *p;
                }
                pointer = (long) file.tellp();
                cout << endl;
            }
        }
    }

    void enumerate_nodes_by_filter(fstream& file, filter_query& query) {
        auto* col = new collection();
        col->header.collection_id = query.collection_id;
        cout << "IN READ operation: " << endl;
        read_collection_header(file, *col);
        if (col->header.is_empty) {
            cout << "no nodes to enumerate " << endl;
        }
        cout << "IN READ COLLECTION: " << endl;
        file.seekp(ios_base::beg);
        long pointer = (long) file.tellp();
        while(pointer != current_pointer) {
            region_header header;
            read_region_header(file, header, pointer);
            if (header.type == COLLECTION) {
                file.seekp(COLLECTION_HEADER_SIZE, ios_base::cur);
                pointer = (long) file.tellp();
            }
            if (header.type == NODE) {
                auto* node = new doc_tree_info();
                node->read_node(file, (long)file.tellg());
                if (node->collection_id != query.collection_id) {
                    long properties_size = (long) (node->real_properties_size* sizeof(property));
                    file.seekp(properties_size, ios_base::cur);
                    pointer = (long) file.tellp();
                    continue;
                }
                cout << "node doc tree: " << endl;
                cout << *node;
                cout << "---" << endl;
                long start_property = (long) file.tellg();
                for (int i = 0; i < node->real_properties_size; i++) {
                    auto* p = new property();
                    p->read_property(file,(long) (start_property + i*sizeof(property)));
                    if (p->property_name != query.filter.property_name) {
                        continue;
                    }
                    switch(query.filter.filter_operator) {
                        case EQUAL:
                            if (p->val == query.filter.value) cout << *p;
                            break;
                        case NOT_EQUAL:
                            if (p->val != query.filter.value) cout << *p;
                            break;
                        case LESS:
                            if (p->val < query.filter.value) cout << *p;
                            break;
                        case MORE:
                            if (p->val > query.filter.value) cout << *p;
                            break;
                        case LESS_OR_EQUAL:
                            if (p->val <= query.filter.value) cout << *p;
                            break;
                        case MORE_OR_EQUAL:
                            if (p->val >= query.filter.value) cout << *p;
                            break;
                    }
                }
                pointer = (long) file.tellp();
                cout << endl;
            }
        }
    }

    void update_nodes_by_filter(fstream& file, filter_query& query) {
        auto* col = new collection();
        col->header.collection_id = query.collection_id;
        cout << "IN READ operation: " << endl;
        read_collection_header(file, *col);
        if (col->header.is_empty) {
            cout << "no nodes to enumerate " << endl;
        }
        cout << "IN READ COLLECTION: " << endl;
        file.seekp(ios_base::beg);
        long pointer = (long) file.tellp();
        while(pointer != current_pointer) {
            region_header header;
            read_region_header(file, header, pointer);
            if (header.type == COLLECTION) {
                file.seekp(COLLECTION_HEADER_SIZE, ios_base::cur);
                pointer = (long) file.tellp();
            }
            if (header.type == NODE) {
                auto* node = new doc_tree_info();
                node->read_node(file, (long)file.tellg());
                if (node->collection_id != query.collection_id) {
                    long properties_size = (long) (node->real_properties_size* sizeof(property));
                    file.seekp(properties_size, ios_base::cur);
                    pointer = (long) file.tellp();
                    continue;
                }
                cout << "node doc tree: " << endl;
                cout << *node;
                cout << "---" << endl;
                long start_property = (long) file.tellg();
                for (int i = 0; i < node->real_properties_size; i++) {
                    auto* p = new property();
                    long before_read_of = (long) file.tellp();
                    p->read_property(file,(long) (start_property + i*sizeof(property)));
                    if (p->property_name != query.filter.property_name) {
                        continue;
                    }
                    p->val = query.filter.value;
                    p->add_property(file, before_read_of); // offset changed new_property ^offset
                }
                pointer = (long) file.tellp();
                cout << endl;
            }
        }
    }

    //2nd param filter for property
    void remove_node_by_filter(fstream& file, filter_query& query) {
        auto* col = new collection();
        col->header.collection_id = query.collection_id;
        cout << "IN READ operation: " << endl;
        read_collection_header(file, *col);
        if (col->header.is_empty) {
            cout << "no nodes to enumerate " << endl;
        }
        cout << "IN READ COLLECTION: " << endl;
        file.seekp(ios_base::beg);
        long pointer = (long) file.tellp();
        while(pointer != current_pointer) {
            region_header header;
            read_region_header(file, header, pointer);
            if (header.type == COLLECTION) {
                file.seekp(COLLECTION_HEADER_SIZE, ios_base::cur);
                pointer = (long) file.tellp();
            }
            if (header.type == NODE) {
                long before_node_offset = (long) file.tellp();
                auto* node = new doc_tree_info();
                node->read_node(file, (long)file.tellg());
                if (node->collection_id != query.collection_id) {
                    long properties_size = (long) (node->real_properties_size* sizeof(property));
                    file.seekp(properties_size, ios_base::cur);
                    pointer = (long) file.tellp();
                    continue;
                }
                long start_property = (long) file.tellg();
                bool is_fit = false;
                for (int i = 0; i < node->real_properties_size; i++) {
                    auto* p = new property();
                    p->read_property(file,(long) (start_property + i*sizeof(property)));
                    if (p->property_name != query.filter.property_name) {
                        continue;
                    }
                    if (p->val != query.filter.value) continue;
                    else is_fit = true;
                }
                if (is_fit) {
                    region::dealloc_node_tree(file, before_node_offset);
                    for (int i = 0; i < node->real_properties_size; i++) {
                        region::dealloc_node_content(file, (long) (start_property + i*sizeof(property)));
                    }
                }
                pointer = (long) file.tellp();
                cout << endl;
            }
        }
    };



};

#endif //LLP_DATABASE_H
