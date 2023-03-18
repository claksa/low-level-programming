#include <iostream>
#include "element.h"
#include <cstring>


Node::Node(int n, node_type v, int p) {
    id = node_id++;
    collection_id = n;
    val = std::move(v);
    parent_id = p;
    if (holds_alternative<string>(val)) val_size = get<string>(val).size();
}

int Node::node_id = 0;

Node::Node() = default;

void Node::set_val_size(int size) {
    val_size = size;
}


int Node::getCollectionId() const {
    return collection_id;
}

void create_node(Node &node) {
    ofstream stream("database.dat", ios::binary | ios::app);
    if (!stream.is_open()) {
        cout << "cannot open file" << endl;
        return;
    } else cout << "file opened" << endl;
    int col = node.getCollectionId();
    int id = node.getId();
    int p_id = node.getParentId();
    stream.write(reinterpret_cast<const char*>(&col), sizeof col);
    stream.write(reinterpret_cast<const char*>(&id), sizeof id);
    string val = visit(make_string_functor(), node.getVal());
    stream.write(val.c_str(), sizeof val.c_str());
    stream.write(reinterpret_cast<const char*>(&p_id), sizeof p_id);
    stream.close();
}

ostream &operator<<(ostream &os, const Node &node) {
//    cout << "-------" << endl;
    cout << "node collection_id: " << node.collection_id;
    cout << "node: " << visit(make_string_functor(), node.val) << endl;
    cout << "parent node: " << node.parent_id << endl;
//    cout << "-------" << endl;
    return os;
}


void read() {
    ifstream stream("database.dat", ios::binary);
    Node node;
    while(stream.read((char*)&node, sizeof(node))) {
        cout << node;
    }
    stream.close();
}

int Node::getId() const {
    return id;
}

const node_type &Node::getVal() const {
    return val;
}

int Node::getParentId() const {
    return parent_id;
}
