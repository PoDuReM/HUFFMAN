#ifndef LIB_TREE_H
#define LIB_TREE_H

#include <map>
#include <vector>
#include <unordered_map>
#include <memory>

class Tree {
public:
    typedef unsigned long long ull;

    Tree() = default;

    explicit Tree(std::vector<ull> &counts);

    void initCur();

    bool go(bool x);

    bool isTerm() const;

    unsigned char getChar() const;

    bool checkEnd() const;

private:
    struct Node;

    std::shared_ptr<Node> root;
    Node *curNode{};
};

#endif //LIB_TREE_H
