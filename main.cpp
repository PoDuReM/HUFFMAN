#include <iostream>
#include <fstream>
#include <time.h>

#include "lib/headers/huffman.h"

void printUsage() {
    std::cout << "usage: ./huffman ⟨-c | -d⟩ source target" << std::endl;
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printUsage();
    }
    auto option = std::string(argv[1]);
    auto source = std::string(argv[2]);
    auto target = std::string(argv[3]);

    std::ifstream in(source, std::ifstream::binary);
    std::ofstream out(target, std::ofstream::binary);
    if (!in.is_open() || !out.is_open()) {
        std::cout << "File opening error\n";
        return 0;
    }

    if (option == "-c") {
        Huffman::compress(in, out);
    } else if (option == "-d") {
        if (!Huffman::decompress(in, out)) {
            std::cout << "File invalid!\n";
        }
    } else {
        printUsage();
    }
    return 0;
}