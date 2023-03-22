#include <stdexcept>
#include <fstream>
#include "scheme_query.h"

unordered_map<int,int> col_pointers;
unordered_map<int, int> node_meta;

scheme_query::scheme_query(int id) {
    collection_id = id;
}

int scheme_query::getCollectionId() const {
    return collection_id;
}

void scheme_query::setCollectionId(int collectionId) {
    collection_id = collectionId;
}

void scheme_query::add_property(const string& key, DataTypes value) {
    props.emplace(key, value);
}

const map<string, DataTypes> &scheme_query::getProps() const {
    return props;
}

ostream &operator<<(ostream &os, const scheme_query &dt) {
    cout << "collection id: " << dt.collection_id << endl;
    for (const auto& [key, value]: dt.getProps()) {
        cout << "property name: " << key << ", type " << ent_str.at(value) << endl;
    }
    return os;
}

ostream &write(ostream &out, scheme_query &scheme) {
    out.write((char*)&scheme.collection_id, sizeof(scheme.collection_id));
    int prop_size = (int)scheme.props.size();
    out.write((char*)&prop_size, sizeof(prop_size));
    for (const auto& [key, value]: scheme.getProps()) {
        int size = (int) key.size();
        out.write((char*)&size, sizeof(size));
        out.write(key.c_str(), size);
        int type = static_cast<int>(value);
        out.write((char*)&type, sizeof(type));
    }
    return out;
}

istream &read(istream &in, scheme_query &scheme) {
    in.seekg(col_pointers.at(scheme.collection_id),ios::beg);
    cout << "pointer before reading scheme: " << in.tellg() << endl;
    in.read((char*)&scheme.collection_id, sizeof(int));
    int col_size;
    in.read((char*)&col_size, sizeof(col_size));
    for (int i = 0; i < col_size; i++) {
        int size;
        in.read((char*)&size, sizeof(size));
        char *tmp = new char[size + 1];
        in.read(tmp, size);
        tmp[size] = '\0';
        int type;
        in.read((char*)&type, sizeof(type));
        scheme.props.insert({tmp,static_cast<DataTypes>(type)});

        delete []tmp;
    }
    return in;
}

void read_collection(fstream &filestream, scheme_query& scheme) {
    if (!filestream.is_open()) return;
    read(filestream, scheme);
    cout << "~~read collection~~ " << endl;
    cout << scheme;
    cout << "pointer after reading scheme: " << filestream.tellg() << endl;
}

void save_collection(fstream &filestream, scheme_query &query) {
    if (!filestream.is_open()) {
        cout << "cannot open file" << endl;
        return;
    }
    int col_pos = filestream.tellp();
    cout << "collection: " << query.getCollectionId() << endl;
    cout << "pointer before writing scheme: " << col_pos << endl;
    col_pointers.emplace(query.getCollectionId(), col_pos);
    write(filestream, query);
    cout << "pointer after writing scheme: " << filestream.tellp() << endl;
    cout << "---" << endl;
}

void create_scheme(fstream &filestream, scheme_query &scheme) {
    save_collection(filestream,scheme);
}

void insert_offset(fstream &filestream, int &offset) {
    filestream.write((char*)&offset, sizeof(offset));
}

// ну типа
void create_node(fstream &filestream, scheme_query &scheme) {
    int collection_id = scheme.getCollectionId();
    read_collection(filestream,scheme);

    map<string, DataTypes> properties = scheme.getProps();

    node_info inf(collection_id, -1);
    string p_name1 = "author", p_name2 = "bookname", p_name3 = "edition";

    element p1({.name = p_name1, .type = properties.at(p_name1)});
    element p2({.name = p_name2, .type = properties.at(p_name2)});
    element p3({.name = p_name3, .type = properties.at(p_name3)});

    p1.add_value("priest");
    p2.add_value("shapolang");
//    p3.add_value(5);
    node_meta.emplace(inf.getId(), (int)filestream.tellp());
    insert_node_info(filestream, inf);
    insert(filestream, p1);
    insert(filestream,p2);
    insert(filestream, p3);
}

void test() {
    fstream filestream(DATABASE, ios::binary | ios::out| ios::in);
    scheme_query scheme(0);
    scheme.add_property("author", STRING_T);
    scheme.add_property("bookname", STRING_T);
    scheme.add_property("edition", INT32_T);
    save_collection(filestream, scheme);

    scheme_query inp_scheme(0);
    create_node(filestream, inp_scheme);
    read_collection(filestream, inp_scheme);
    filestream.close();
}
