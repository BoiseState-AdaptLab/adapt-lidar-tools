// File name: CsvWriter.cpp
// Created on: 17-July-2019
// Author: Spencer Fleming


#include "CsvWriter.hpp"

#include <iostream>
#include <fstream>

/**
 * Default constructor
 */
CsvWriter::CsvWriter() {
    lines = NULL;
}

void CsvWriter::setLines(std::vector<std::string*>* strings){
    this->lines = strings;
}

std::vector<std::string*>* CsvWriter::getLines() {
    return this->lines;
}

/**
 * Deletes every string in Lines, and then deletes the vector pointed to
 * by lines.
 */
void CsvWriter::freeLines() {
    for (auto i = this->lines->begin(); i != this->lines->end(); ++i) {
        delete (*i);
    }
    delete this->lines;
}

/**
 * Writes csv file to file named by filename
 * @param filename file to populate with csv data
 * @returns 0 if successful, 1 if file did not open
 */
int CsvWriter::write_to_csv(std::string filename) {
    std::ofstream file (filename);
    if (file.is_open()) {
        for (auto i = this->lines->begin(); i != this->lines->end(); ++i) {
            file << (**i);
            file << '\n';
        }
        file.close();
    } else {
        //File could not be opened
        return 1;
    }
    return 0;
}
