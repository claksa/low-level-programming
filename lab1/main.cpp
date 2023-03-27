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
    auto* col1 = new collection(sch);
    db.create_collection(filestream, *col);
    db.create_collection(filestream, *col1);
    auto* tmp_col = new collection();
    tmp_col->header.collection_id = col1->header.collection_id;
    db.read_collection_header(filestream, *tmp_col);
    db.delete_collection(filestream, col1->header.collection_id);
    auto* col3 = new collection(sch);
    db.create_collection(filestream, *col3);
    tmp_col->header.collection_id = col3->header.collection_id;
    db.read_collection_header(filestream, *tmp_col);
    database::close(filestream);
    return 0;
}