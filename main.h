#ifndef LLP_MAIN_H
#define LLP_MAIN_H

#include <iostream>
#include <string>
#include <variant>
#include <vector>
#include <map>
#include <any>
#include <unordered_map>
#include <stdexcept>

using namespace std;

using node_type = variant<int32_t, float, string, bool>;

struct make_string_functor {
    std::string operator()(const std::string &x) const { return x; }
    std::string operator()(::int32_t x) const { return to_string(x); }
    std::string operator()(float x) const { return to_string(x); }
    std::string operator()(bool x) const { return to_string(x); }
};

class Node {
public:
    bool leaf = false; // leaf == "child" or just node without children
    node_type value;
    string node_id;
//    vector<Node*> children;
//    Node* parent;
};

enum DataTypes {INT32_T, FLOAT_T, STRING_T, BOOL_T};

static unordered_map<string, DataTypes> const str_ent = {
        {"int32", DataTypes::INT32_T},
        {"float", DataTypes::FLOAT_T},
        {"string", DataTypes::STRING_T},
        {"bool", DataTypes::BOOL_T}};

static unordered_map<DataTypes, string> const ent_str = {
        {DataTypes::INT32_T, "int32"},
        {DataTypes::FLOAT_T, "float"},
        {DataTypes::STRING_T, "string"},
        {DataTypes::BOOL_T, "bool"}};

DataTypes getEnumDataType(string &type) {
    auto it = str_ent.find(type);
    if (it != str_ent.end()) {
        return it->second;
    } else {
        throw invalid_argument("unknown val: " + type);
    }
}

unordered_map<string, vector<string>> pr_constraints; // node_name=id --> node's properties (children)
unordered_map<string, DataTypes> type_constraints;

//database as example for checking
vector<Node> database;


bool is_schema(string &schema_name) {
    if (type_constraints.count(schema_name) == 1) {
        cout << "val constraints for " << schema_name << " are existed" << endl;
    } else {
        cout << "val constraints for " << schema_name << " aren't existed" << endl;
        return false;
    }

    // иметь в виду что так-то свойств может и не быть
    if (pr_constraints.count(schema_name) == 1) {
        cout << "properties constraints for " << schema_name << " are existed" << endl;
    } else {
        cout << "properties constraints for " << schema_name << " aren't existed" << endl;
        return false;
    }
    return true;
}

bool is_valid_type(node_type &val, DataTypes &type) {
    bool is_right_type = false;
    switch(type) {
        case INT32_T:
            is_right_type = holds_alternative<::int32_t>(val);
            if (!is_right_type) cout << "value should be int32_t val" << endl;
            return is_right_type;
        case FLOAT_T:
            is_right_type = holds_alternative<float>(val);
            if (!is_right_type) cout << "value should be float val" << endl;
            return is_right_type;
        case STRING_T:
            is_right_type = holds_alternative<string>(val);
            if (!is_right_type) cout << "value should be string " << endl;
            return is_right_type;
        case BOOL_T:
            is_right_type = holds_alternative<bool>(val);
            if (!is_right_type) cout << "value should be bool val" << endl;
            return is_right_type;
        default:
            cout << "unknown data val" << endl;
            return is_right_type;
    }
}

struct element_query {
    string node_name;
    node_type val;
    string parent_name;
};

struct scheme_query {
    string scheme_name;
    DataTypes scheme_type;
    bool is_parent = false;
    vector<scheme_query> children;
};


bool is_child_property(string &node, string &parent_node) {
    vector<string> properties = pr_constraints.at(parent_node);
    for (const auto& prop: properties) {
        if (prop == node) {
            cout << "according to the data schema, the parent " << parent_node <<
                 " has the child " << prop << endl;
            return true;
        }
    }
    return false;
}

void create_element(struct element_query *query) {
    Node node;
    if (is_schema(query->node_name)) {
        auto node_type = type_constraints.at(query->node_name);
        if (!is_valid_type(query->val, node_type)) return;
        cout << "data val for node " << query->node_name << "is " << ent_str.at(node_type);
        cout << endl;
    } else return;
    if (!query->parent_name.empty()) {
        if (is_schema(query->parent_name)) {
            cout << "node " << query->parent_name << "existed and has got child nodes " << endl;
            if (!is_child_property(query->node_name, query->parent_name)) {
                cout << "according to the data schema, the parent " << query->parent_name <<
                     " does not have a child " << query->node_name << endl;
                // raise exception
            }
        }
    } else node.leaf = true;
    node.value = query->val;
    node.node_id = query->node_name;
    database.push_back(node);
}

void create_scheme(scheme_query *query) {
    if (is_schema(query->scheme_name)) {
        cout << "property " << query->scheme_name << " has already existed" << endl;
        return;
    }
    type_constraints.emplace(query->scheme_name, query->scheme_type);
    vector<scheme_query> children;
    vector<string> children_names;
    if (query->is_parent) {
        children = query->children;
        for (auto child: children) {
            create_scheme(&child);
            children_names.push_back(child.scheme_name);
        }
    }
    pr_constraints.emplace(query->scheme_name, children_names);
}

void print_vector(vector<string> &vec) {
    for (auto v: vec) {
        cout << v << ' ';
    }
}

void test() {
    scheme_query sch1, sch2, sch3, sch4;
    element_query q1, q2, q3, q4;
    sch1.scheme_name = "book";
    q1.node_name = sch1.scheme_name;
    q1.val = "my_book";

    sch2.scheme_name = "author";
    q2.node_name = sch2.scheme_name;
    q2.val = "priest";
    q2.parent_name = sch1.scheme_name;

    sch3.scheme_name = "bookname";
    q3.node_name = sch3.scheme_name;
    q3.val = "shapolang";
    q3.parent_name = sch1.scheme_name;

    sch4.scheme_name = "edition";
    q4.node_name = sch4.scheme_name;
    q4.val = 5;
    q4.parent_name = sch4.scheme_name;

    sch1.scheme_type = STRING_T;
    sch2.scheme_type = STRING_T;
    sch3.scheme_type = STRING_T;
    sch4.scheme_type = INT32_T;

    sch1.is_parent = true;
    vector<scheme_query> ch_nodes;
    ch_nodes.push_back(sch2);
    ch_nodes.push_back(sch3);
    ch_nodes.push_back(sch4);

    sch1.children = ch_nodes;

    create_scheme(&sch1);

    cout << "property constraints: "<< endl;
    for (auto p: pr_constraints) {
        cout << "key: " << p.first << ", value: ";
        print_vector(p.second);
        cout << endl;
    }

    create_element(&q1);
    create_element(&q2);
    create_element(&q3);
    create_element(&q4);

    for (auto node: database) {
        cout << "node " << node.node_id << endl;
        cout << "value [" << visit(make_string_functor(), node.value) << "]\n";
    }

}


#endif //LLP_MAIN_H
