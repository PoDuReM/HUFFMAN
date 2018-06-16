#include <queue>
#include <utility>
#include <algorithm>
#include "headers/tree.h"

struct Tree::Node {
    unsigned char letter{};
    std::shared_ptr<Node> children[2];

    Node() = default;

    explicit Node(unsigned char _letter, std::shared_ptr<Node> _L = nullptr, std::shared_ptr<Node> _R = nullptr) :
            letter(_letter), children{std::move(_L), std::move(_R)} {}
};

Tree::Tree(std::vector<ull> &counts) {
    auto *curNodes = new std::pair<ull, std::shared_ptr<Node>>[257];
    auto *addNodes = new std::pair<ull, std::shared_ptr<Node>>[256];
    size_t iC = 0, iA = 0, cC = 0, cA = 0;
    addNodes[iA] = {(ull) -1, nullptr};
    curNodes[iC++] = {counts[0], std::make_shared<Node>(0)};
    curNodes[iC++] = {counts[1], std::make_shared<Node>(1)};
    for (unsigned char i = 2; i > 0; ++i) {
        if (counts[i]) {
            curNodes[iC++] = {counts[i], std::make_shared<Node>(i)};
        }
    }
    std::sort(curNodes, curNodes + iC, [](auto a, auto b) {
        return a.first == b.first ? a.second->letter < b.second->letter : a.first < b.first;
    });
    curNodes[iC] = {(ull) -1, nullptr};
    auto getMin = [&cA, &cC, &iA, &curNodes, &addNodes]() {
        return curNodes[cC].first == addNodes[cA].first ?
               curNodes[cC].second->letter < addNodes[cA].second->letter ? curNodes[cC++] : addNodes[cA++]
               : curNodes[cC].first < addNodes[cA].first ? curNodes[cC++] : addNodes[cA++];
    };
    while (iC - cC + iA - cA > 1) {
        auto X = getMin();
        auto Y = getMin();
        addNodes[iA++] = {X.first + Y.first, std::make_shared<Node>(X.second->letter, X.second, Y.second)};
        addNodes[iA] = {(ull) -1, nullptr};
    }
    root = cA < iA ? addNodes[cA].second : curNodes[cC].second;
    delete[] curNodes;
    delete[] addNodes;
}

void Tree::initCur() {
    curNode = root.get();
}

bool Tree::go(bool x) {
    if (curNode->children[0] == nullptr) {
        return false;
    }
    curNode = curNode->children[x].get();
    return true;
}

unsigned char Tree::getChar() const {
    return curNode->letter;
}

bool Tree::isTerm() const {
    return curNode->children[0] == nullptr;
}

bool Tree::checkEnd() const {
    return curNode == root.get();
}
