#include <iostream>

#include "gtest/gtest.h"
#include "../headers/huffman.h"

TEST(correctness, empty) {
    std::stringstream in("");
    std::stringstream comp;
    std::stringstream dec;

    Huffman::compress(in, comp);
    Huffman::decompress(comp, dec);
    EXPECT_EQ(in.str(), dec.str());
}

TEST(correctness, one_char) {
    std::stringstream in("#");
    std::stringstream comp;
    std::stringstream dec;

    Huffman::compress(in, comp);
    Huffman::decompress(comp, dec);
    EXPECT_EQ(in.str(), dec.str());
}

TEST(correctness, sentence) {
    std::stringstream in("I can do it.");
    std::stringstream comp;
    std::stringstream dec;

    Huffman::compress(in, comp);
    Huffman::decompress(comp, dec);
    EXPECT_EQ(in.str(), dec.str());
}

TEST(correctness, all_chars) {
    std::stringstream in;
    std::stringstream comp;
    std::stringstream dec;

    for (auto i = CHAR_MIN; i <= CHAR_MAX; ++i) {
        in << (char)i;
    }

    Huffman::compress(in, comp);
    Huffman::decompress(comp, dec);
    EXPECT_EQ(in.str(), dec.str());
}

TEST(correctness, big_file) {
    for (int i = 0; i < 4; ++i) {
        std::stringstream in;
        std::stringstream comp;
        std::stringstream dec;

        for (auto i = 0; i < 1024 * 1024; ++i) {
            char b = rand() % 256;
            in << b;
        }

        Huffman::compress(in, comp);
        Huffman::decompress(comp, dec);
        EXPECT_EQ(in.str(), dec.str());
    }
}

TEST(correctness, invalid_decoder) {
    std::stringstream comp;
    std::stringstream dec;
    comp << "#34233";
    EXPECT_EQ(false, Huffman::decompress(comp, dec));
}

TEST(correctness, all_same_char) {
    std::stringstream in;
    std::stringstream comp;
    std::stringstream dec;

    for (auto i = 0; i < 1024 * 1024; ++i) {
        char b = 'a';
        in << b;
    }

    Huffman::compress(in, comp);
    Huffman::decompress(comp, dec);
    EXPECT_EQ(in.str(), dec.str());
}

TEST(correctness, two_chars) {
    std::stringstream in;
    std::stringstream comp;
    std::stringstream dec;

    char a[2] = { 'a', 'b' };
    for (auto i = 0; i < 1024 * 1024; ++i) {
        in << a[rand() % 2];
    }

    Huffman::compress(in, comp);
    Huffman::decompress(comp, dec);
    EXPECT_EQ(in.str(), dec.str());
}

TEST(correctness, compress_compressed) {
    for (int i = 0; i < 4; ++i) {
        std::stringstream in;
        std::stringstream comp;
        std::stringstream comp2;
        std::stringstream dec2;
        std::stringstream dec;

        for (auto i = 0; i < 1024 * 1024; ++i) {
            char b = rand() % 256;
            in << b;
        }

        Huffman::compress(in, comp);
        Huffman::compress(comp, comp2);
        Huffman::decompress(comp2, dec2);
        Huffman::decompress(dec2, dec);
        EXPECT_EQ(in.str(), dec.str());
    }
}