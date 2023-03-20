#include <iostream>
#include "element.h"
#include <unordered_map>
#include <cstring>
#include <utility>

unordered_map<int,int> node_pointers;

ostream &operator<<(ostream &os, const element &node) {
    cout << "------" << endl;
    cout << "node id: " << node.id << endl;
    cout << "properties (values): " << endl;
    for (const auto& value: node.properties) {
        cout << visit(make_string_functor(), value.getValue());
        cout << ' ';
    }
    cout << "------" << endl;
    return os;
}
static int el_id = 0;

element::element(int p_id) {
    id = el_id++;
    parent_id = p_id;
}

element::element() {
    id = el_id++;
}

int element::getId() const {
    return id;
}


ostream &write(ostream &out, element &node) {
    out.write((char*)&node.id, sizeof(node.id));
    out.write((char*)&node.parent_id, sizeof(node.parent_id));
    int size = (int)node.properties.size();
    out.write((char*)&size, sizeof(size));
    for (const auto& val: node.properties) {
        int name_size = (int)val.getName().size();
        out.write((char*)&name_size, sizeof(name_size));
        out.write(val.getName().c_str(), name_size);
        int type = static_cast<int>(val.getType());
        out.write((char*)&type, sizeof(type));
        if (holds_alternative<string>(val.getValue())) {
            string s = get<string>(val.getValue());
            int ns = (int)s.size();
            out.write((char*)&ns, sizeof(ns));
            out.write(s.c_str(), ns);
        }
        if (holds_alternative<::int32_t>(val.getValue())) {
            ::uint32_t v = get<int32_t>(val.getValue());
            int ns = (int) sizeof(::int32_t);
            out.write((char*)&ns, sizeof(ns));
            out.write((char*)&v, ns);
        }
        if (holds_alternative<float>(val.getValue())) {
            float v = get<float>(val.getValue());
            int ns = (int) sizeof(v);
            out.write((char*)&ns, sizeof(ns));
            out.write((char*)&v, ns);
        }
        if (holds_alternative<bool>(val.getValue())) {
            bool v = get<bool>(val.getValue());
            int ns = (int) sizeof(v);
            out.write((char*)&ns, sizeof(ns));
            out.write((char*)&v, ns);
        }
    }
    return out;
}


void insert(element &node) {
    ofstream filestream(DATABASE, ios::binary | ios::out);
    if (!filestream.is_open()) {
        cout << "cannot open file" << endl;
        return;
    } else cout << "file opened" << endl;
    int cur_pos = (int)filestream.tellp();
    cout << "pointer before write node: " << cur_pos << endl;
    write(filestream, node);
    cout << "pointer after write node: " << filestream.tellp() << endl;
    filestream.close();
}

// AAAAAAAAAAAAA
istream &read(istream &in, element &node) {
    in.read((char*)&node.id, sizeof(node.id));
    cout << "read node_id: " << node.id << endl;
    in.read((char*)&node.parent_id, sizeof(node.parent_id));
    cout << "read parent_id: " << node.parent_id << endl;
    int size;
    in.read((char*)&size, sizeof(size));
    node.properties.resize(size);
    for (int i = 0; i < size; i++) {
        int name_size;
        in.read((char*)&name_size, sizeof(name_size));
        char *tmp = new char[name_size + 1];
        in.read(tmp, name_size);
        tmp[name_size] = '\0';
        node.properties[i].setName(tmp);
        delete []tmp;

        int type;
        in.read((char*)&type, sizeof(type));
        node.properties[i].setType(static_cast<DataTypes>(type));

        int len;
        in.read((char*)&len, sizeof(len));
        switch (node.properties[i].getType()) {
            case INT32_T:
                ::int32_t data;
                in.read((char*)&data, len);
                node.properties[i].setValue(data);
                break;
            case FLOAT_T:
                float num;
                in.read((char*)&num, len);
                node.properties[i].setValue(num);
                break;
            case BOOL_T:
                bool b;
                in.read((char*)&b, len);
                node.properties[i].setValue(b);
                break;
            case STRING_T:
                char* tmp = new char[len + 1];
                in.read(tmp, len);
                tmp[len] = '\0';
                node.properties[i].setValue(tmp);
                delete []tmp;
                break;
        }
    }
    return in;
}

void element::add_property(property& p) {
    properties.push_back(p);
}

int element::getParentId() const {
    return parent_id;
}

const vector<property> &element::getProperties() const {
    return properties;
}

void read_node(element &node) {
    ifstream filestream(DATABASE, ios::binary | ios::in);
    cout << "pointer before reading: " << filestream.tellg() << endl;
    if (!filestream.is_open()) {
        cout << "cannot open file" << endl;
        return;
    } else cout << "file opened" << endl;
    read(filestream, node);
    filestream.close();
    cout << "pointer after reading: " << filestream.tellg() << endl;
    cout << "----" << endl;
    cout << "id: " << node.getId() << endl;
    cout << "parent_id: " << node.getParentId() << endl;
    for (const auto& p: node.getProperties()) {
        cout << p;
    }
}

ostream &operator<<(ostream &os, const property &pr) {
    cout << "property name: " << pr.name << endl;
    cout << "type: " << ent_str.at(pr.type) << endl;
    cout << "value: " << visit(make_string_functor(), pr.value) << endl;
    return os;
}

void property::setName(const string &name) {
    property::name = name;
}

void property::setType(DataTypes type) {
    property::type = type;
}

void property::setValue(const node_type &value) {
    property::value = value;
}

property::property(string n, DataTypes type, node_type node) {
    name = std::move(n);
    this->type = type;
    value = std::move(node);
}

const string &property::getName() const {
    return name;
}

DataTypes property::getType() const {
    return type;
}

const node_type &property::getValue() const {
    return value;
}
