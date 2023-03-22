#ifndef LLP_ELEMENT_H
#define LLP_ELEMENT_H

#include <utility>
#include <vector>
#include <fstream>
#include "type.h"

#define DATABASE "database.dat"


using namespace std;

struct node_scheme {
    string name;
    DataTypes type;
};

class node_info {
    int collection_id;
    int id;
    int parent_id;
    int properties;
    inline static int el_id = 0;

public:
    node_info(int collection_id, int parent_id);

    [[nodiscard]] int getId() const;
    [[nodiscard]] int getProperties() const;
    void setProperties(int properties);

    friend ostream& write(ostream& out, node_info& inf);
    friend istream& read(istream& in, node_info& inf);
};

struct document {

};

class element {
    node_scheme scheme;
    node_type val;

    inline static int num = 0;
public:
    element() = default;
    explicit element(node_scheme sch);
    void add_value(node_type value);

    friend ostream& operator<<(ostream& os, const element& node);
    friend ostream& write(ostream& out, element& node);
    friend istream& read(istream& in, element& node);
};

void insert_node_info(fstream &filestream, node_info &inf);
void read_node_info(fstream &filestream, node_info &inf);

void insert(fstream &filestream, element& node);
void read_node(fstream &filestream, element &node);
#endif //LLP_ELEMENT_H
