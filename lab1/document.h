#ifndef LLP_DOCUMENT_H
#define LLP_DOCUMENT_H

#include <utility>
#include <vector>
#include <fstream>
#include "type.h"


using namespace std;

class doc_tree_info {
    int node_id;
    int parent_id;
//    int children_n;
    static inline int id;
    string node_name;
//    long* children_offsets;

public:
    doc_tree_info(int parent_id, string name) :
    node_id(id++),
    parent_id(parent_id),
//    children_n(children_n),
    node_name(std::move(name))
//    children_offsets(new long[children_n])
    {}

    friend ostream& write(ostream& out, doc_tree_info& node) {
        out.write(reinterpret_cast<const char*>(&node), sizeof(node));
    }

    friend istream& read(istream& in, doc_tree_info& node) {
        in.read(reinterpret_cast<char*>(&node), sizeof(node));
        return in;
    }

    friend ostream& operator<<(ostream& os, const doc_tree_info& node) {
        cout << "node_id: " << node.node_id << endl;
        cout << "parent_id: " << node.parent_id << endl;
    }
};

class property {
    string property_name;
    node_type val;
public:
    explicit property(string name, node_type value) :
    property_name(std::move(name)),
    val(std::move(value)) {}

    friend ostream& write(ostream& out, property& property) {
        out.write(reinterpret_cast<const char*>(&property), sizeof(property));
    }

    friend istream& read(istream& in, property& property) {
        in.read(reinterpret_cast<char*>(&property), sizeof(property));
        return in;
    }

    friend ostream& operator<<(ostream& os, const property& property) {
        cout << "property name: " << property.property_name << endl;
        cout << "value: " << visit(make_string_functor(), property.val) << endl;
    }
};

#endif //LLP_DOCUMENT_H
