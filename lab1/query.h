
#ifndef LLP_QUERY_H
#define LLP_QUERY_H
#include <string>
#include "type.h"

using namespace std;

struct add_node_query {
    long collection_id;
    string node_name;
    node_type* properties;
    long parent_id;
    int children_size;
    long* children;
};

enum OPERATION {
    UPDATE,
    REMOVE
//    ENUMERATE
};

enum FILTER {
    EQUAL,
    NOT_EQUAL,
    LESS,
    MORE,
    LESS_OR_EQUAL,
    MORE_OR_EQUAL
};

struct filter {
//    string node_name;
    string property_name;
    node_type value; // can be new value if it is update op
    FILTER filter_operator; //optional
};

struct filter_query {
    long collection_id;
    filter filter;
};


#endif //LLP_QUERY_H
