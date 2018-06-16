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

    std::vector<std::pair<unsigned char, ull>> getCodes() const;

    void initCur();

    bool go(bool x);

    bool isTerm() const;

    unsigned char getChar() const;

    bool checkEnd() const;

private:
    struct Node;

    std::shared_ptr<Node> root, curNode;

    void dfs(std::shared_ptr<Node> R, std::vector<std::pair<unsigned char, ull>> &res, ull curCode, unsigned char len) const;
};

#endif //LIB_TREE_H
