#ifndef LLP_TESTS_H
#define LLP_TESTS_H

#include "database.h"

void test_file() {
    fstream file;
    file.open(R"(C:\Users\DELL\fall3_labs\low-level-programming\lab1\aa.dat)", ios::binary|ios::out|ios::in);
    if (file.is_open()) {
        cout << "opened file" << endl;
    }
    cout << file.tellp() << endl;
    file.close();
}

void test_collection() {
    fstream filestream;
    database db((database_info()));
    db.open(filestream);
    filestream.seekp(0, ios_base::beg);
    int property_nums = 2;
    scheme sch{property_nums, "book"};
    sch.field[0] = {.data_type = 2, .property_name = "author"};
    sch.field[1] = {.data_type = 2, .property_name = "name"};
    auto* col = new collection(sch);
    db.create_collection(filestream, *col);
    db.delete_collection(filestream, col->header.collection_id);

    auto* read_col = new collection();
    read_col->header.collection_id = col->header.collection_id;
    db.read_collection_header(filestream, *read_col);
    database::close(filestream);
}

void test_node_insert_and_read_all() {
    fstream filestream;
    database db((database_info()));
    db.open(filestream);
    int property_nums = 2;
    scheme sch{property_nums, "book"};
    sch.field[0] = {.data_type = 2, .property_name = "author"};
    sch.field[1] = {.data_type = 2, .property_name = "name"};


    auto* col = new collection(sch);
    auto* col2 = new collection(sch);
    db.create_collection(filestream, *col);
    db.create_collection(filestream, *col2);
    node_type val1 = "priest";
    node_type val2 = "shapolang";
    node_type val3 = "Liu Yao";
    node_type types[2] = {val1, val2};
    node_type types1[2] = {val1, val3};
    add_node_query query = {.collection_id = col->header.collection_id,
            .node_name = "my_book",
            .properties = types1,
            .parent_id = -1,
            .children_size = 0,
            .children = {}};
    db.create_node(filestream, &query);
    add_node_query query1 = {.collection_id = col2->header.collection_id,
            .node_name = "book2",
            .properties = types,
            .parent_id = -1,
            .children_size = 0,
            .children = {}};
    db.create_node(filestream, &query1);
    db.read_all_docs_database(filestream);
    db.read_collection_documents(filestream, col2->header.collection_id);
    database::close(filestream);
}

#endif //LLP_TESTS_H
