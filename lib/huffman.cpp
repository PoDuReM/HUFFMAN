#include <fstream>
#include "headers/huffman.h"
#include <vector>
#include <algorithm>
#include <cstring>
#include <queue>

void Huffman::compress(std::istream &in, std::ostream &out) {
    in.seekg(0, std::istream::end);
    auto len = (size_t) in.tellg(), cur = len;
    in.seekg(std::istream::beg);
    const size_t buff_size = 2048;
    unsigned char buff[buff_size];

    std::vector<ull> counts(256, 0);
    unsigned char x = 0;
    while (cur) {
        size_t auxSize = std::min(buff_size, cur);
        cur -= auxSize;
        in.read((char *) buff, auxSize);
        for (size_t i = 0; i < auxSize; ++i) {
            ++counts[buff[i]];
        }
    }

    unsigned char outBuff[buff_size];
    size_t curOut = 0;
    auto writer = [&curOut, &outBuff, &buff_size, &out](auto x) {
        if (curOut + sizeof x >= buff_size) {
            out.write((char *) outBuff, curOut);
            curOut = 0;
        }
        memcpy(outBuff + curOut, &x, sizeof x);
        curOut += sizeof x;
    };

    writer(x);
    short countSymbols = 2;
    for (unsigned char i = 2; i > 0; ++i)
        if (counts[i]) {
            ++countSymbols;
        }
    writer(countSymbols);
    writer((unsigned char) 0);
    writer(counts[0]);
    writer((unsigned char) 1);
    writer(counts[1]);
    for (unsigned char i = 2; i > 0; ++i)
        if (counts[i]) {
            writer(i);
            writer(counts[i]);
        }

    std::vector<std::pair<unsigned char, ull>> codes = getCodes(counts);
    in.seekg(std::istream::beg);

    unsigned char curCntBit = 0;
    ull curBit = 0;
    cur = len;
    while (cur) {
        size_t auxSize = std::min(buff_size, cur);
        cur -= auxSize;
        in.read((char *) buff, auxSize);
        for (size_t i = 0; i < auxSize; ++i) {
            curBit |= codes[buff[i]].second << curCntBit;
            curCntBit += codes[buff[i]].first;
            if (curCntBit >= 64) {
                writer(curBit);
                curCntBit -= 64;
                curBit = codes[buff[i]].second >> (codes[buff[i]].first - curCntBit);
            }
        }
    }

    if (curCntBit) {
        curCntBit = (unsigned char) (64 - curCntBit);
        writer(curBit);
    }

    if (curOut) {
        out.write((char *) outBuff, curOut);
    }
    out.seekp(0);
    out.write((char *) &curCntBit, sizeof curCntBit);
}

bool Huffman::decompress(std::istream &in, std::ostream &out) {
    in.seekg(0, std::istream::end);
    auto cur = (size_t) in.tellg();
    in.seekg(std::istream::beg);
    unsigned char countInvalid = 0;
    in.read((char *) &countInvalid, sizeof countInvalid);
    cur -= sizeof countInvalid;
    if (!in) {
        return false;
    }

    std::vector<ull> counts(256, 0);
    short countSymbols = 0;
    in.read((char *) (&countSymbols), sizeof countSymbols);
    cur -= sizeof countSymbols;
    for (short i = 0; i < countSymbols; ++i) {
        unsigned char key = 0;
        ull value = 0;
        if (!in) {
            return false;
        }
        in.read((char *) &key, sizeof key);
        cur -= sizeof key;
        if (!in) {
            return false;
        }
        in.read((char *) (&value), sizeof value);
        cur -= sizeof value;
        counts[key] = value;
    }

    const size_t buff_size = 2048;
    unsigned char buff[buff_size];

    unsigned char outBuff[buff_size];
    size_t curOut = 0;
    auto writer = [&curOut, &outBuff, &buff_size, &out](auto x) {
        if (curOut + sizeof x >= buff_size) {
            out.write((char *) outBuff, curOut);
            curOut = 0;
        }
        memcpy(outBuff + curOut, &x, sizeof x);
        curOut += sizeof x;
    };

    Tree huffTree(counts);
    huffTree.initCur();
    while (cur) {
        size_t auxSize = std::min(buff_size, cur);
        cur -= auxSize;
        in.read((char *) buff, auxSize);
        for (size_t j = 0; j < auxSize; j += 8) {
            ull x, cntBits = 64;
            if (j + 8 > auxSize) {
                return false;
            }
            memcpy(&x, buff + j, sizeof x);
            if (!cur && j + 8 == auxSize) {
                cntBits -= countInvalid;
            }
            for (unsigned char i = 0; i < cntBits; ++i) {
                if (!huffTree.go((bool) ((x >> i) & 1))) {
                    return false;
                }
                if (huffTree.isTerm()) {
                    unsigned char y = huffTree.getChar();
                    writer(y);
                    huffTree.initCur();
                }
            }
        }
    }
    if (curOut) {
        out.write((char *) outBuff, curOut);
    }
    return huffTree.checkEnd();
}

std::vector<std::pair<unsigned char, Huffman::ull>> Huffman::getCodes(std::vector<ull> &counts) {
    std::vector<std::pair<unsigned char, ull>> res(256, {0, 0});
    auto *curNodes = new std::pair<ull, std::vector<unsigned char>>[257];
    auto *addNodes = new std::pair<ull, std::vector<unsigned char>>[256];
    size_t iC = 0, iA = 0, cC = 0, cA = 0;
    addNodes[iA] = {(ull) -1, {}};
    curNodes[iC++] = {counts[0], {0}};
    curNodes[iC++] = {counts[1], {1}};
    for (unsigned char i = 2; i > 0; ++i) {
        if (counts[i]) {
            curNodes[iC++] = {counts[i], {i}};
        }
    }
    std::sort(curNodes, curNodes + iC);
    curNodes[iC] = {(ull) -1, {}};
    auto getMin = [&cA, &cC, &curNodes, &addNodes]() {
        return curNodes[cC] < addNodes[cA] ? curNodes[cC++] : addNodes[cA++];
    };
    while (iC - cC + iA - cA > 1) {
        auto X = getMin();
        auto Y = getMin();
        for (auto c : X.second) {
            res[c].first++;
            res[c].second <<= 1;
        }
        for (auto c : Y.second) {
            res[c].first++;
            (res[c].second <<= 1)++;
        }
        std::vector<unsigned char> aux(X.second.size() + Y.second.size());
        X.second.insert(X.second.end(), Y.second.begin(), Y.second.end());
        addNodes[iA++] = {X.first + Y.first, X.second};
        addNodes[iA] = {(ull) -1, {}};
    }
    delete[] curNodes;
    delete[] addNodes;
    return res;
}

/*
* countInvlaidLastBits - char
*
* sizeOfMap - short int
* <key-value> - <char, ull>
*
* ...compressedFile  byte - <char>
*/