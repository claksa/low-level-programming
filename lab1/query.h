
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

struct remove_node_query {
    long collection_id;
    string node_name;
};

#endif //LLP_QUERY_H
