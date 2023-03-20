#ifndef LLP_ELEMENT_H
#define LLP_ELEMENT_H

#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <fstream>
#include <unordered_map>

#define DATABASE "database.dat"


using namespace std;

enum DataTypes {INT32_T, FLOAT_T, STRING_T, BOOL_T};

static unordered_map<DataTypes, string> const ent_str = {
        {DataTypes::INT32_T, "int32_t"},
        {DataTypes::FLOAT_T, "float"},
        {DataTypes::STRING_T, "string"},
        {DataTypes::BOOL_T, "bool"}};

typedef variant<int32_t, float, string, bool> node_type;

struct make_string_functor {
    string operator()(const std::string &x) const { return x; }
    string operator()(::int32_t x) const { return to_string(x); }
    string operator()(float x) const { return to_string(x); }
    string operator()(bool x) const { return to_string(x); }
};

class property {
    string name;
    DataTypes type;
    node_type value;
    friend ostream& operator<<(ostream& os, const property& pr);
public:
    property() = default;
    property(string n, DataTypes type, node_type node);
    void setName(const string &name);
    [[nodiscard]] const string &getName() const;
    void setType(DataTypes type);
    [[nodiscard]] DataTypes getType() const;
    void setValue(const node_type &value);
    [[nodiscard]] const node_type &getValue() const;
};

class element {
//    int collection_id; under question
    int id;
    int parent_id = -1; // or just save offset

    vector<property> properties; //idk is it optimal
public:
    element();
    explicit element(int p_id);
    void add_property(property& p);
    friend ostream& operator<<(ostream& os, const element& node);
    friend ostream& write(ostream& out, element& node);
    friend istream& read(istream& in, element& node);

    [[nodiscard]] int getId() const;
    [[nodiscard]] int getParentId() const;
    [[nodiscard]] const vector<property> &getProperties() const;
};

void insert(element& node);
void read_node(element &node);
#endif //LLP_ELEMENT_H
