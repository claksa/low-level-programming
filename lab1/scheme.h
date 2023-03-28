#ifndef LLP_SCHEME_H
#define LLP_SCHEME_H


#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <map>
#include "document.h"
#include "type.h"


using namespace std;

struct property_field {
    int data_type{};
    string property_name;
};


class scheme {
public:
    string collection_name;
    int property_nums;
    property_field* field;


    explicit scheme(int property_nums, string name) :
    property_nums(property_nums),
    collection_name(std::move(name)),
    field(new property_field[property_nums])
    {}

     ~scheme() {
//        delete []field;
    }

    friend ostream& write(ostream& out, scheme& sch) {
        out.write(reinterpret_cast<char*>(&sch), sizeof(sch));
        return out;
    }

    friend istream& read(istream& in, scheme& sch) {
        in.read(reinterpret_cast<char*>(&sch), sizeof(sch));
        return in;
    }

    friend ostream& operator<<(ostream& os, const scheme& sch) {
        cout << "collection name: " << sch.collection_name << endl;
        for (int i = 0; i < sch.property_nums; i++) {
            cout << "property name: "<< sch.field[i].property_name << endl;
            auto type = static_cast<DataTypes>(sch.field[i].data_type);
            cout << "type: " << ent_str.at(type) << endl;
        }
        return os;
    }

    void write_scheme_info(ostream &filestream) {
        write(filestream, *this);
    }

    void read_scheme_info(istream& filestream) {
        read(filestream, *this);
    }

};

#endif //LLP_SCHEME_H
