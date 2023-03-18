#ifndef LLP_ELEMENT_H
#define LLP_ELEMENT_H

#include <string>
#include <utility>
#include <variant>
#include <vector>
#include <fstream>

using namespace std;

typedef variant<int32_t, float, string, bool> node_type;

struct make_string_functor {
    string operator()(const std::string &x) const { return x; }
    string operator()(::int32_t x) const { return to_string(x); }
    string operator()(float x) const { return to_string(x); }
    string operator()(bool x) const { return to_string(x); }
};

class Node {
    int collection_id;
    int id;
    node_type val;
    int parent_id;
    int val_size = 0;
public:
    static int node_id;
    Node();
    Node(int n, node_type v, int p);
    friend ostream& operator<<(ostream& os, const Node& node);
    void set_val_size(int size);

    [[nodiscard]] int getId() const;
    [[nodiscard]] const node_type &getVal() const;
    [[nodiscard]] int getParentId() const;
    [[nodiscard]] int getCollectionId() const;
};

void create_node(Node &node);
void read();

#endif //LLP_ELEMENT_H
