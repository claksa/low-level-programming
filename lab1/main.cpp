#include "scheme_query.h"
#include "element.h"


void test_node() {
    int col_id = 0;
    Node* node_ptr = new Node(col_id, "book1", -1);
    Node* node_ptr1 = new Node(col_id, "priest", node_ptr->getId());
    Node* node_ptr2 = new Node(col_id, 5, 0);
    Node* node_ptr3 = new Node(col_id, "shapolang", node_ptr->getId());
    create_node(*node_ptr);
//    read();
    delete node_ptr;
    delete node_ptr1;
    delete node_ptr2;
    delete node_ptr3;
}

int main() {
    test_node();
//    test_scheme();
    return 0;
}