#include "database.h"


int main() {
    fstream filestream;
    database db((database_info()));
    db.open(filestream);
    int property_nums = 2;
    scheme sch{property_nums};
    sch.field[0] = {.data_type = 2, .property_name = "author"};
    sch.field[1] = {.data_type = 2, .property_name = "name"};


    auto* col = new collection(sch);
    auto* col1 = new collection(sch);
    db.create_collection(filestream, *col);
    db.create_collection(filestream, *col1);
    db.read_collection(filestream, col->header.collection_id);
    db.read_collection(filestream, col1->header.collection_id);
    database::close(filestream);
    return 0;
}