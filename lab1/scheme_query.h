#ifndef LLP_SCHEME_QUERY_H
#define LLP_SCHEME_QUERY_H


#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <map>
#include "element.h"

void test();

using namespace std;

class scheme_query {
    int collection_id;
    map<string , DataTypes> props;
public:
    scheme_query() = default;
    explicit scheme_query(int id);

    [[nodiscard]] int getCollectionId() const;
    [[nodiscard]] const map<string, DataTypes> &getProps() const;

    void setCollectionId(int collectionId);
    void add_property(const string& key, DataTypes value);

    friend ostream& operator<<(ostream& os, const scheme_query& dt);
    friend ostream& write(ostream& out, scheme_query& scheme);
    friend istream& read(istream& in, scheme_query& scheme);
};

#endif //LLP_SCHEME_QUERY_H
