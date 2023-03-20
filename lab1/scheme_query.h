#ifndef LLP_SCHEME_QUERY_H
#define LLP_SCHEME_QUERY_H


#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "element.h"

using namespace std;

void test();


static unordered_map<string, DataTypes> const str_ent = {
        {"int32", DataTypes::INT32_T},
        {"float", DataTypes::FLOAT_T},
        {"string", DataTypes::STRING_T},
        {"bool", DataTypes::BOOL_T}};


struct node_scheme {
    string el;
    DataTypes el_type;
};

// book: name string, author string, edition int32 -- save in db
class scheme_query {
    int collection_id;
    vector<node_scheme> properties;
public:
    scheme_query() = default;
    scheme_query(int col, vector<node_scheme> props);

    int getCollectionId() const;
    void setCollectionId(int collectionId);

    friend ostream& operator<<(ostream& os, const scheme_query& dt);
    friend ostream& write(ostream& out, scheme_query& scheme);
    friend istream& read(istream& in, scheme_query& scheme);
};

#endif //LLP_SCHEME_QUERY_H
