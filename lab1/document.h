#ifndef LLP_DOCUMENT_H
#define LLP_DOCUMENT_H

#include <utility>
#include <vector>
#include <fstream>
#include "type.h"


using namespace std;

class doc_tree_info {
    int parent_id;
    static inline int id;
public:
    int children_n;
    int real_properties_size;
    long* children{};
    string node_name;
    int node_id;
    long start_content_offset;

    doc_tree_info() :
    node_id(-1),
    parent_id(-2),
    node_name(""),
    start_content_offset(0),
    real_properties_size(0)
    {}

    doc_tree_info(int parent_id, string name, long start_prop, int children_n, int p_num) :
    node_id(id++),
    parent_id(parent_id),
    node_name(std::move(name)),
    start_content_offset(start_prop),
    children_n(children_n),
    children(new long[children_n]),
    real_properties_size(p_num)
    {}

    friend ostream& write(ostream& out, doc_tree_info& node) {
        out.write(reinterpret_cast<char*>(&node), sizeof(node));
        return out;
    }

    void add_node(fstream& filestream, long offset) {
        filestream.seekp(offset, ios_base::beg);
        write(filestream, *this);
    }

    friend istream& read(istream& in, doc_tree_info& node) {
        in.read(reinterpret_cast<char*>(&node), sizeof(node));
        return in;
    }

    void read_node(fstream& filestream, long offset) {
//        filestream.seekg(offset, ios_base::beg);
        read(filestream, *this);
    }

    friend ostream& operator<<(ostream& os, const doc_tree_info& node) {
        cout << "node_id: " << node.node_id << endl;
        cout << "node_name: " << node.node_name << endl;
        cout << "parent_id: " << node.parent_id << endl;
        for (int i = 0; i < node.children_n; i++) {
            cout << "children id: " << node.children[i] << endl;
        }
        return os;
    }
};

class property {
    string property_name;
    node_type val;
public:
    property():
    property_name(""),
    val(0)
    {}
    property(string name, node_type value, DataTypes type) {
        property_name = std::move(name);
        if (is_valid_type(value, type)) {
            val = value;
        } else {
            // throw exception
            cout << "error validation" << endl;
            ::exit(1);
        }
    }

    friend ostream& write(ostream& out, property& property) {
        out.write(reinterpret_cast<char*>(&property), sizeof(property));
        return out;
    }

    long add_property(fstream& filestream, long offset) {
        filestream.seekp(offset, ios_base::beg);
        write(filestream, *this);
        return (long) filestream.tellp();
    }

    friend istream& read(istream& in, property& property) {
        in.read(reinterpret_cast<char*>(&property), sizeof(property));
        return in;
    }

    void read_property(fstream& file, long offset) {
        file.seekg(offset, ios_base::beg);
        read(file, *this);
    }

    friend ostream& operator<<(ostream& os, const property& property) {
        cout << "property name: " << property.property_name << endl;
        cout << "value: " << visit(make_string_functor(), property.val) << endl;
        return os;
    }
};

#endif //LLP_DOCUMENT_H
