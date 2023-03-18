#include <stdexcept>
#include <fstream>
#include <utility>
#include "scheme_query.h"

DataTypes get_enum_data_type(string &type) {
    auto it = str_ent.find(type);
    if (it != str_ent.end()) {
        return it->second;
    } else {
        throw invalid_argument("unknown val: " + type);
    }
}

bool is_valid_type(node_type &val, DataTypes &type) {
    bool is_right_type = false;
    switch(type) {
        case INT32_T:
            is_right_type = holds_alternative<::int32_t>(val);
            if (!is_right_type) cout << "value should be int32_t val" << endl;
            return is_right_type;
        case FLOAT_T:
            is_right_type = holds_alternative<float>(val);
            if (!is_right_type) cout << "value should be float val" << endl;
            return is_right_type;
        case STRING_T:
            is_right_type = holds_alternative<string>(val);
            if (!is_right_type) cout << "value should be string " << endl;
            return is_right_type;
        case BOOL_T:
            is_right_type = holds_alternative<bool>(val);
            if (!is_right_type) cout << "value should be bool val" << endl;
            return is_right_type;
        default:
            cout << "unknown data val" << endl;
            return is_right_type;
    }
}

scheme_query::scheme_query(int col, vector<node_scheme> props) {
    collection_id = col;
    properties = std::move(props);
}

void test_scheme() {
    int collection = 0;

    vector<node_scheme> nodes;
    node_scheme val = {.el= "book", .el_type =  STRING_T};
    // nodes for book collection
    // leaf == true for properties
    node_scheme pr1 = {.el = "name", .leaf = true, .el_type = STRING_T};
    node_scheme pr2 = {.el = "author", .leaf = true, .el_type = STRING_T};
    node_scheme pr3 = {.el = "edition", .leaf = true, .el_type = INT32_T};

    nodes.push_back(val);
    nodes.push_back(pr1);
    nodes.push_back(pr2);
    nodes.push_back(pr3);

    scheme_query* col1 = new scheme_query(collection, nodes);
    delete col1;
}

