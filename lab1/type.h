#ifndef LLP_TYPE_H
#define LLP_TYPE_H


#include <unordered_map>
#include <string>
#include <variant>

enum region_type {
    COLLECTION,
    NODE
};

struct region_header {
    region_type type;
    bool is_removed;
};

enum DataTypes {INT32_T, FLOAT_T, STRING_T, BOOL_T};

static std::unordered_map<DataTypes, std::string> const ent_str = {
        {DataTypes::INT32_T, "int32_t"},
        {DataTypes::FLOAT_T, "float"},
        {DataTypes::STRING_T, "string"},
        {DataTypes::BOOL_T, "bool"}};

static std::unordered_map<std::string, DataTypes> const str_ent = {
        {"int32", DataTypes::INT32_T},
        {"float", DataTypes::FLOAT_T},
        {"string", DataTypes::STRING_T},
        {"bool", DataTypes::BOOL_T}};


typedef std::variant<int32_t, float, std::string, bool> node_type;

struct make_string_functor {
    std::string operator()(const std::string &x) const { return x; }
    std::string operator()(::int32_t x) const { return std::to_string(x); }
    std::string operator()(float x) const { return std::to_string(x); }
    std::string operator()(bool x) const { return std::to_string(x); }
};

bool is_valid_type(node_type &val, DataTypes &type);

#endif //LLP_TYPE_H
