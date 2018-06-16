#include <queue>
#include "headers/tree.h"

struct Tree::Node {
    unsigned char letter;
    Node *L, *R;

    Node() = default;

    explicit Node(unsigned char _letter, Node *_L = nullptr, Node *_R = nullptr) :
            letter(_letter), L(_L), R(_R) {}
};

std::vector<std::pair<unsigned char, Tree::ull>> Tree::getCodes() const {
    std::vector<std::pair<unsigned char, ull>> res(256, { 0, 0 });
    dfs(root, res, 0, 0);
    return res;
}

Tree::Tree(std::vector<ull> &counts) {
    class cmp {
    public:
        bool operator() (std::pair<ull, Node*> a, std::pair<ull, Node*> b) {
            return a.first > b.first;
        }
    };
    std::priority_queue<std::pair<ull, Node *>, std::vector<std::pair<ull, Node *>>, cmp> curNodes;
    curNodes.push({ counts[0], new Node((unsigned char)0) });
    curNodes.push({ counts[1], new Node((unsigned char)1) });
    for (unsigned char i = 2; i > 0; ++i) if (counts[i]) {
        curNodes.push({ counts[i], new Node(i) });
    }
    while (curNodes.size() > 1) {
        auto X = curNodes.top();
        curNodes.pop();
        auto Y = curNodes.top();
        curNodes.pop();
        curNodes.push({ X.first + Y.first, new Node('-', X.second, Y.second) });
    }
    root = curNodes.top().second;
    curNodes.pop();
}

void Tree::myDelete(Node *R) {
    if (R->L != nullptr) {
        myDelete(R->L);
        myDelete(R->R);
    }
    delete R;
}

Tree::~Tree() {
    myDelete(root);
}

void Tree::initCur() {
    curNode = root;
}

bool Tree::go(bool x) {
    if (curNode->L == nullptr) {
        return false;
    }
    if (x) {
        curNode = curNode->R;
    } else {
        curNode = curNode->L;
    }
    return true;
}

unsigned char Tree::getChar() const {
    return curNode->letter;
}

bool Tree::isTerm() const {
    return curNode->L == nullptr;
}

void Tree::dfs(Tree::Node *R, std::vector<std::pair<unsigned char, ull>> &res, ull curCode, unsigned char len) const {
    if (R->L == nullptr) {
        res[R->letter] = { len, curCode };
        return void();
    }
    dfs(R->L, res, curCode, (unsigned char)(len + 1));
    curCode |= ((ull)1 << len);
    dfs(R->R, res, curCode, (unsigned char)(len + 1));
}

bool Tree::checkEnd() const {
    return curNode == root;
}
