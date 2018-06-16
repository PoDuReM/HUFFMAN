#ifndef LIB_HUFFMAN_H
#define LIB_HUFFMAN_H

#include "tree.h"

class Huffman {
public:
    typedef unsigned long long ull;

    Huffman() = default;

    static void compress(std::istream &in, std::ostream &out);

    static bool decompress(std::istream &out, std::ostream &in);
};

#endif //LIB_HUFFMAN_H
