#ifndef LIB_TREE_H
#define LIB_TREE_H

#include <map>
#include <vector>
#include <unordered_map>

class Tree {
public:
    typedef unsigned long long ull;

    Tree() = default;

    Tree(std::vector<ull> &counts);

    ~Tree();

    std::vector<std::vector<bool>> getCodes() const;

    void initCur();

    bool go(bool x);

    bool isTerm() const;

    unsigned char getChar() const;

    bool checkEnd() const;

private:
    struct Node;

    Node *root, *curNode;

    void myDelete(Node *R);

    void dfs(Node *R, std::vector<std::vector<bool>> &res, std::vector<bool> &curCode) const;
};

#endif //LIB_TREE_H
