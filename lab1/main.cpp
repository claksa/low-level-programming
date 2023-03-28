#include "database.h"


int main() {
    fstream filestream;
    database db((database_info()));
    db.open(filestream);
    int property_nums = 2;
    scheme sch{property_nums, "book"};
    sch.field[0] = {.data_type = 2, .property_name = "author"};
    sch.field[1] = {.data_type = 2, .property_name = "name"};


    auto* col = new collection(sch);
    db.create_collection(filestream, *col);
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
    add_node_query query1 = {.collection_id = col->header.collection_id,
                             .node_name = "book2",
                             .properties = types,
                             .parent_id = -1,
                             .children_size = 0,
                             .children = {}};
    db.create_node(filestream, &query1);
    db.read_all_nodes(filestream, col->header.collection_id);
    database::close(filestream);
    return 0;
}