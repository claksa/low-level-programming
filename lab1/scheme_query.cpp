#include <stdexcept>
#include <fstream>
#include <utility>
#include <cstring>
#include "scheme_query.h"

unordered_map<int,int> col_pointers;

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

int scheme_query::getCollectionId() const {
    return collection_id;
}

void scheme_query::setCollectionId(int collectionId) {
    collection_id = collectionId;
}

ostream &operator<<(ostream &os, const scheme_query &dt) {
    cout << dt.collection_id << endl;
    for (const auto& i: dt.properties){
        cout << i.el << " : " << ent_str.at(i.el_type) << endl;
    }
    return os;
}

ostream &write(ostream &out, scheme_query &scheme) {
    out.write((char*)&scheme.collection_id, sizeof(scheme.collection_id));
    int prop_size = (int)scheme.properties.size();
    out.write((char*)&prop_size, sizeof(prop_size));
    for (node_scheme& item: scheme.properties) {
        string property = item.el;
        out.write(property.c_str(), property.length());
        out.put(' ');
    }
    return out;
}

istream &read(istream &in, scheme_query &scheme) {
    in.seekg(col_pointers.at(scheme.collection_id),ios::beg);
    cout << "pointer before reading: " << in.tellg() << endl;
    in.read((char*)&scheme.collection_id, sizeof(int));
    int col_size;
    in.read((char*)&col_size, sizeof(col_size));
    scheme.properties.resize(col_size);
    for (node_scheme &item: scheme.properties) {
        getline(in, item.el, ' ');
        cout << "property name: " << item.el << endl;
    }
    cout << "pointer after reading: " << in.tellg() << endl;
    in.seekg(0);
    return in;
}

void read_collection(scheme_query& scheme) {
    ifstream filestream(DATABASE, ios::binary | ios::in);
    if (!filestream.is_open()) return;
    read(filestream, scheme);
    cout << "read collection id: " << scheme.getCollectionId() << endl;
    filestream.close();
}

void save_collection(scheme_query &query) {
    ofstream filestream(DATABASE, ios::binary | ios::out);
    if (!filestream.is_open()) {
        cout << "cannot open file" << endl;
        return;
    } else cout << "file opened" << endl;
    int col_pos = filestream.tellp();
    cout << "collection: " << query.getCollectionId() << endl;
    cout << "position before writing: " << col_pos << endl;
    col_pointers.emplace(query.getCollectionId(), col_pos);
    write(filestream, query);
    cout << "position after writing: " << filestream.tellp() << endl;
    cout << "---" << endl;
    filestream.close();
}

void delete_collection(scheme_query &query) {

}

void test() {
    int collection = 0, collection1 = 1;

    vector<node_scheme> nodes;
    vector<node_scheme> nodes1;

    // nodes for book collection
    // leaf == true for properties
    // leaf vs property: leaf --> collection, property --> node. but property is leaf as well
    node_scheme pr1 = {.el = "bookname", .el_type = STRING_T};
    node_scheme pr2 = {.el = "author", .el_type = STRING_T};
    node_scheme pr3 = {.el = "edition", .el_type = INT32_T};

    nodes.push_back(pr1);
    nodes.push_back(pr2);
    nodes.push_back(pr3);

    nodes1.push_back(pr2);

    scheme_query col1(collection, nodes1);
    scheme_query col2(collection1, nodes);
    scheme_query col;

    property p1("bookname",STRING_T,"shapolang");
    property p2("author", STRING_T, "priest");
    property p3("edition", INT32_T, 5);
    element node(-1);
    node.add_property(p1);
    node.add_property(p2);
    node.add_property(p3);

    insert(node);
    element node1;
    read_node(node1);
}

