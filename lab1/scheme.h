#ifndef LLP_SCHEME_H
#define LLP_SCHEME_H


#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <map>
#include "element.h"


using namespace std;

struct property_field {
    int data_type{};
    string property_name;
};


class scheme {
public:
    int property_nums;
    property_field* field = nullptr; // or just (?) vector


    explicit scheme(int property_nums) : property_nums(property_nums)
    {
        field = new property_field[property_nums];
    }

     ~scheme() {
//        delete []field;
    }

    friend ostream& write(ostream& out, scheme& sch) {
        cout << "property scheme size: " << sizeof(sch) << endl;
        out.write((char*)&sch, sizeof(sch));
        return out;
    }

    friend istream& read(istream& in, scheme& sch) {
        in.read((char*)&sch, sizeof(sch));
        return in;
    }

    friend ostream& operator<<(ostream& os, const scheme& sch) {
        for (int i = 0; i < sch.property_nums; i++) {
            cout << "property name: "<< sch.field[i].property_name << endl;
            cout << "type: " << sch.field[i].data_type << endl;
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
