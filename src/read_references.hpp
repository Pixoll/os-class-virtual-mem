#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/**
 * Read references from filename (2.4)
 * @param filename Name of the file
 * @return The references
 */
inline std::vector<int> read_references(const std::string &filename) {
    std::ifstream file(filename);
    std::vector<int> references;
    int number;

    if (!file.is_open()) {
        std::cerr << "Error while opening file: " << filename << std::endl;
        exit(1);
    }

    std::cout << "Reading file: " << filename << std::endl;

    while (file >> number) {
        references.push_back(number);
    }

    return references;
}
