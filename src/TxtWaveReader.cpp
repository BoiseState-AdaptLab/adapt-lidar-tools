#define STR_MAX_DEFAULT 100
#define C_NAME typeid(*this).name()

#include <string>
#include <cstring>

#include "spdlog/spdlog.h"

#include "TxtWaveReader.hpp"

TxtWaveReader::TxtWaveReader () {
    idx.clear();
    wave.clear();
    maxWaveLen = STR_MAX_DEFAULT;
    line = (char*) malloc(maxWaveLen * sizeof(char));
    line[0] = '\0';
    line_count = 0;
}

TxtWaveReader::TxtWaveReader (int maxWaveLen) {
    idx.clear();
    wave.clear();
    line = (char*) malloc(maxWaveLen * sizeof(char));
    line[0] = '\0';
    line_count = 0;
}

/**
 * opens the text file.
 */
int TxtWaveReader::open_file (const char* filename) {
    txtFile.open(filename);
    return !txtFile;
}

/**
 * Checks if current line read from file contains only digits 0-9 and space.
 * @return true if the line is valid, false otherwise.
 */
bool TxtWaveReader::is_line_valid() {
    for (int i = 0; i < maxWaveLen; i++) {
        if (line[i] == '\0') return true;
        if ((line[i] < '0' || line[i] > '9') && (line[i] != ' ')) {
            return false;
        }
    }
    return true;
}

/**
 * Reads a line from text file and converts it to a vector of integers.
 * @param array std::vector<int> to place data in
 * @return False if end of file is reached, true otherwise.
 */
bool TxtWaveReader::get_vector (std::vector<int>& vect) {
    vect.clear();

    // If last read set eof bit, return false.
    if (txtFile.eof()) {
        spdlog::debug("{}: stream at eof", C_NAME);
        return false;
    }

    // Else, we are good to read the next line.
    txtFile.getline (line, maxWaveLen);

    line_count++;
    spdlog::trace("{}: reading line {}", C_NAME, line_count);

    // Makes sure an error has not occured during the last read.
    if (txtFile.bad() || txtFile.fail()) {
        spdlog::debug("{}: bad read on line {}", C_NAME, line_count);
        return true;
    }

    // Makes sure it is a sequence of digits 0-9 and space.
    bool valid = is_line_valid();
    if (!valid) {
        spdlog::debug("{}: bad format read at line {}", C_NAME,
                line_count);
        return true;
    }

    char *pos = strtok (line, " ");
    while (pos != NULL) {
        std::string str(pos);
        vect.push_back(std::stoi(str, NULL));
        pos = strtok(0, " ");
    }

    // Send warning if vector is empty
    if (vect.empty()) {
        spdlog::warn("{}: empty line read at line {}", C_NAME,
                line_count);
    }

    return true;
}

/**
 * Reads next pulse from text file. Errors in reading will result in empty
 * vectors, and an entry in the logs specifying what went wrong.
 * @return False if end of file was reached, true otherwise.
 */
bool TxtWaveReader::next_wave () {
    bool status_idx = get_vector(idx);
    bool status_wave = get_vector(wave);

    return ( status_idx && status_wave );
}

TxtWaveReader::~TxtWaveReader () {
    txtFile.close();
    free(line);
}
