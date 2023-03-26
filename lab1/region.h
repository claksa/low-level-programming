#ifndef LLP_REGION_H
#define LLP_REGION_H

#include <cstring>
#include "element.h"
#include "collection.h"
#include "meta_db_info.h"

#define NODE_INFO_SIZE sizeof(element)*DEFAULT_DOCS_NUM
#define NODE_TREE_INFO_SIZE sizeof(node_info)
#define COLLECTION_HEADER_SIZE sizeof(collection)


enum ALLOC_RES {
    SUCCESS,
    FAILED
};

class region {

public:
    char col_buf[COLLECTION_HEADER_SIZE]{};
    char doc_buf[NODE_INFO_SIZE]{};
    char doc_tree_buf[NODE_TREE_INFO_SIZE]{};



    // TODO RES HANDLING
    // always hehe success

    ALLOC_RES allocate_collection_region(ostream &file, long offset) {
        file.seekp(offset, ios_base::beg);
        ::memset(col_buf, 0, sizeof(col_buf));
        file.write(col_buf, sizeof(col_buf));
        return SUCCESS;
    }

    ALLOC_RES allocate_doc_region(ostream &file, long offset) {
        file.seekp(offset, ios::beg);
        ::memset(doc_buf, 0, sizeof(doc_buf));
        file.write(doc_buf, sizeof(doc_buf));
        return SUCCESS;
    }

    ALLOC_RES allocate_doc_tree_region(ostream &file, long offset) {
        file.seekp(offset, ios::beg);
        ::memset(doc_tree_buf, 0, sizeof(doc_tree_buf));
        file.write(doc_tree_buf, sizeof(doc_tree_buf));
        return SUCCESS;
    }

};


#endif //LLP_REGION_H
