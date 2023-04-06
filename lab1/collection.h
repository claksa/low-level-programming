#ifndef LLP_COLLECTION_H
#define LLP_COLLECTION_H

#include <iosfwd>
#include <iostream>
#include "scheme.h"
#include "query.h"

using namespace std;

class collection_meta_info {
public:
    long start_documents;
    long end_documents; // also start of tree model records
    long end_tree_model_recs;
    bool is_empty;
    long collection_id;
    long offset_from_beg;
    static inline long current_id;

    long end_of_last_property;
    long end_of_last_node;

    collection_meta_info() = default;

    collection_meta_info(long offsetFromBeg, long start_docs, long end_docs, long end_coll) :
            collection_id(current_id++),
            offset_from_beg(offsetFromBeg),
            start_documents(start_docs),
            end_documents(end_docs),
            end_tree_model_recs(end_coll),
            is_empty(true),
            end_of_last_property(start_docs),
            end_of_last_node(end_docs)
    {}

    friend ostream& operator<<(ostream& os, const collection_meta_info& meta_inf) {
        cout << "collection_id: " << meta_inf.collection_id << endl;
        cout << "offset_from_beg: " << meta_inf.offset_from_beg << endl;
        cout << "start_documents: " << meta_inf.start_documents << endl;
        cout << "end_documents: " << meta_inf.end_documents << endl;
        cout << "end_tree_model_res: " << meta_inf.end_tree_model_recs << endl;
        cout << "empty collection: " << meta_inf.is_empty << endl;
        return os;
    }
};



class collection {
public:
    //size_t cur_num_docs; // dynamically  changed -- пока опционально, не сохраняю
    collection_meta_info header;
    scheme sch;
    explicit collection(const scheme& sch) : sch(sch), header({}){}

    collection(const collection_meta_info &header, const scheme &scheme) :
            header(header), sch(scheme) {}

    collection(): sch({}, ""), header({}) {}


public:
    friend ostream& write(ostream& out, collection& col) {
        out.write(reinterpret_cast<char*>(&col.header), sizeof(col.header));
        col.sch.write_scheme_info(out);
        return out;
    }

    friend istream& read(istream& in, collection& col) {
        in.read(reinterpret_cast<char*>(&col.header), sizeof(col.header));
        col.sch.read_scheme_info(in);
        return in;
    }

    void write_collection(ostream& out) {
        out.seekp(this->header.offset_from_beg, ios_base::beg);
        write(out, *this);
    }

    void read_collection(istream& in, long offset) {
        in.seekg(offset, ios::beg);
        cout << "offset before READ collection: " << offset << endl;
        read(in, *this);
    }

    friend ostream& operator<<(ostream& os, const collection& meta_inf) {
        cout << meta_inf.header << endl;
        cout << "--collection scheme--" << endl;
        cout << meta_inf.sch;
        return os;
    }

    void set_header(const collection_meta_info &header) {
        collection::header = header;
    }

    long insert_node(fstream& file, doc_tree_info& node) {
        long node_tree_start = header.end_of_last_node;
        node.add_node(file, node_tree_start);
        return (long) file.tellp();
    }

    //!!!NOT TREE TRAVERSION
    void read_node_tree(fstream& file) const {
        cout << "READ NODE_TREE: " << endl;
        long offset = header.end_documents; // and start node tree info
        file.seekp(offset, ios_base::beg);
        while ((long)file.tellp() != header.end_of_last_node)  {
            auto* node = new doc_tree_info();
            node->read_node(file, offset);
            cout << *node;
            cout << "---" << endl;

            long start_property = node->start_content_offset;
            for (int i = 0; i < node->real_properties_size; i++) {
                auto* p = new property();
                p->read_property(file,(long) (start_property + i*sizeof(property)));
                cout << *p;
            }

            cout << endl;
            offset = offset + (long) sizeof(doc_tree_info);
            file.seekp(offset, ios_base::beg);
        }
    }

    void enumerate_by_filter(fstream& file, filter& filter) const {
        node_type value = filter.value;
        long offset = header.end_documents; // and start node tree info
        file.seekp(offset, ios_base::beg);
        while ((long)file.tellp() != header.end_of_last_node)  {
            auto* node = new doc_tree_info();
            node->read_node(file, offset);
            cout << *node;
            cout << "---" << endl;

            long start_property = node->start_content_offset;
            for (int i = 0; i < node->real_properties_size; i++) {
                auto* p = new property();
                p->read_property(file,(long) (start_property + i*sizeof(property)));
                if (p->property_name != filter.property_name) continue;

                switch(filter.filter_operator) {
                    case EQUAL:
                        if (p->val == filter.value) cout << *p;
                        break;
                    case NOT_EQUAL:
                        if (p->val != filter.value) cout << *p;
                        break;
                    case LESS:
                        if (p->val < filter.value) cout << *p;
                        break;
                    case MORE:
                        if (p->val > filter.value) cout << *p;
                        break;
                    case LESS_OR_EQUAL:
                        if (p->val <= filter.value) cout << *p;
                        break;
                    case MORE_OR_EQUAL:
                        if (p->val >= filter.value) cout << *p;
                        break;
                }
            }

            cout << endl;
            offset = offset + (long) sizeof(doc_tree_info);
            file.seekp(offset, ios_base::beg);
        }
    }

    void update_by_filter(fstream &file, filter& filter) {
        // rewrite
    }

};


#endif //LLP_COLLECTION_H
