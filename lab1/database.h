
#ifndef LLP_DATABASE_H
#define LLP_DATABASE_H

#include <cstddef>
#include <utility>
#include "scheme.h"
#include "region.h"


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

    void create_collection(fstream &filestream, collection &col) {
        long col_offset = end_of_last_collection;
        cout << "collection offset: " << col_offset << endl;
        region reg;

        // TODO make it abstract
        // allocate region for collection header
        reg.allocate_collection_region(filestream, col_offset);
        cout << "pointer after collection allocation: " << filestream.tellp() << endl;
        long start_documents = (long) filestream.tellp();

        // allocate region for possible documents
        reg.allocate_doc_region(filestream, start_documents);
        long end_documents = (long) filestream.tellp();
        cout << "pointer after docs allocation: " << end_documents << endl;

        // allocate region for document tree
        reg.allocate_doc_tree_region(filestream, end_documents);
        long end_collection = (long) filestream.tellp();
        cout << "pointer after docs_tree allocation: " << end_collection << endl;
        this->end_of_last_collection = end_collection;

        col.set_header(collection_meta_info (col_offset,
                                             start_documents,
                                             end_documents,
                                             end_collection));

        col.write_collection(filestream);
        cout << "pointer after write collection: " << filestream.tellp() << endl;
    }

    void read_collection_header(fstream &file, long collection_id) {
        long offset = DB_OFFSET + collection_id*col_docs_size;
        cout << "offset before reading: " << offset << endl;
        cout << "read collection with id=" << collection_id << endl;
        collection col;
        col.read_collection(file, offset);
        cout << col;
    }

};

#endif //LLP_DATABASE_H
