#include <iostream>
#include "element.h"
#include <unordered_map>
#include <utility>

unordered_map<int,int> node_pointers;

node_info::node_info(int collection_id, int parent_id) {
    this->collection_id = collection_id;
    id = el_id++;
    this->parent_id = parent_id;
}

ostream &write(ostream &out, node_info &inf) {
    out.write((char*)&inf.collection_id, sizeof(inf.collection_id));
    out.write((char*)&inf.id, sizeof(inf.id));
    out.write((char*)&inf.parent_id, sizeof(inf.parent_id));
    out.write((char*)&inf.properties, sizeof(inf.properties));
    return out;
}

void insert_node_info(fstream &filestream, node_info &inf) {
    write(filestream, inf);
}

istream &read(istream &in, node_info &inf) {
    in.read((char*)&inf.collection_id, sizeof(inf.collection_id));
    in.read((char*)&inf.id, sizeof(inf.id));
    in.read((char*)&inf.parent_id, sizeof(inf.parent_id));
    in.read((char*)&inf.properties, sizeof(inf.properties));
    return in;
}

int node_info::getId() const {
    return id;
}

int node_info::getProperties() const {
    return properties;
}

void node_info::setProperties(int properties) {
    node_info::properties = properties;
}

void read_node_info(fstream &filestream, node_info &inf) {
    read(filestream, inf);
}

element::element(node_scheme sch) {
    scheme = std::move(sch);
}

void element::add_value(node_type value) {
    if (is_valid_type(value, this->scheme.type)) {
        val = value;
    }
}

ostream &operator<<(ostream &os, const element &node) {
    cout << "property name: " << node.scheme.name << endl;
    cout << "value: " << visit(make_string_functor(), node.val) << endl;
    return os;
}


ostream &write(ostream &out, element &node) {
    int size = (int)node.scheme.name.size();
    out.write((char*)&size, sizeof(size));
    out.write(node.scheme.name.c_str(), size);
    int type = static_cast<int>(node.scheme.type);
    out.write((char*)&type, sizeof(type));
    if (holds_alternative<string>(node.val)) {
        string s = get<string>(node.val);
        int ns = (int)s.size();
        out.write((char*)&ns, sizeof(ns));
        out.write(s.c_str(), ns);
    }
    if (holds_alternative<::int32_t>(node.val)) {
        ::uint32_t v = get<int32_t>(node.val);
        int ns = (int) sizeof(::int32_t);
        out.write((char*)&ns, sizeof(ns));
        out.write((char*)&v, ns);
    }
    if (holds_alternative<float>(node.val)) {
        float v = get<float>(node.val);
        int ns = (int) sizeof(v);
        out.write((char*)&ns, sizeof(ns));
        out.write((char*)&v, ns);
    }
    if (holds_alternative<bool>(node.val)) {
        bool v = get<bool>(node.val);
        int ns = (int) sizeof(v);
        out.write((char*)&ns, sizeof(ns));
        out.write((char*)&v, ns);
    }
    return out;
}

void insert(fstream &filestream, element &node) {
    if (!filestream.is_open()) {
        cout << "cannot open file" << endl;
        return;
    }
    int cur_pos = (int)filestream.tellp();
    cout << "pointer before write node: " << cur_pos << endl;
    write(filestream, node);
    cout << "pointer after write node: " << filestream.tellp() << endl;
}

istream &read(istream &in, element &node) {
    int size;
    in.read((char*)&size, sizeof(size));

    char *tmp = new char[size + 1];
    in.read(tmp, size);
    tmp[size] = '\0';
    node.scheme.name = tmp;
    delete []tmp;

    int type;
    in.read((char*)&type, sizeof(type));
    node.scheme.type = static_cast<DataTypes>(type);

    int len;
    in.read((char*)&len, sizeof(len));

    switch (node.scheme.type) {
        case INT32_T:
            ::int32_t data;
            in.read((char*)&data, len);
            node.val = data;
            break;
        case FLOAT_T:
            float num;
            in.read((char*)&num, len);
            node.val = num;
            break;
        case BOOL_T:
            bool b;
            in.read((char*)&b, len);
            node.val = b;
            break;
        case STRING_T:
            char* str = new char[len + 1];
            in.read(str, len);
            str[len] = '\0';
            node.val = str;
            delete []str;
            break;
    }
    return in;
}

void read_node(fstream &filestream, element &node) {
    cout << "pointer before reading node: " << filestream.tellg() << endl;
    if (!filestream.is_open()) {
        cout << "cannot open file" << endl;
        return;
    };
    read(filestream, node);
    cout << "pointer after reading node: " << filestream.tellg() << endl;
    cout << "----" << endl;
}
