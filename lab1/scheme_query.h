#ifndef LLP_SCHEME_QUERY_H
#define LLP_SCHEME_QUERY_H


#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include "element.h"

using namespace std;

void test_scheme();

enum DataTypes {INT32_T, FLOAT_T, STRING_T, BOOL_T};

static unordered_map<DataTypes, string> const ent_str = {
        {DataTypes::INT32_T, "int32_t"},
        {DataTypes::FLOAT_T, "float"},
        {DataTypes::STRING_T, "string"},
        {DataTypes::BOOL_T, "bool"}};

struct node_scheme {
    string el;
    bool leaf = false;
    DataTypes el_type;
};


// book: name string, author string, edition int32 -- save in db
class scheme_query {
private:
    int collection_id; // collection_id name, optional
    vector<node_scheme> properties;
public:
    scheme_query(int col, vector<node_scheme> props);
    friend ostream& operator<<(ostream& os, const scheme_query& dt) {
        cout << dt.collection_id << endl;
        for (auto i: dt.properties){
            cout << i.el << " : " << ent_str.at(i.el_type) << endl;
        }
        return os;
    }
//    void save(ofstream& of);
};


static unordered_map<string, DataTypes> const str_ent = {
        {"int32", DataTypes::INT32_T},
        {"float", DataTypes::FLOAT_T},
        {"string", DataTypes::STRING_T},
        {"bool", DataTypes::BOOL_T}};



#endif //LLP_SCHEME_QUERY_H
