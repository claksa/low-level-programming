#include <stdexcept>
#include <iostream>
#include "type.h"

using namespace std;

string get_string_data_type(DataTypes &type) {
    auto it = ent_str.find(type);
    if (it != ent_str.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("unknown datatype");
    }
}

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


