#define STR_MAX_DEFAULT 100

#include <string>
#include <cstring>

#include "TxtWaveReader.hpp"

TxtWaveReader::TxtWaveReader () {
    maxWaveLen = STR_MAX_DEFAULT;
    line = (char*) malloc(maxWaveLen * sizeof(char));
    line[0] = '\0';
}

TxtWaveReader::TxtWaveReader (int maxWaveLen) {
    line = (char*) malloc(maxWaveLen * sizeof(char));
    line[0] = '\0';
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
 * @return 0 if things go fine, 1 if badbit is set after line read or if the
 * line recieved contains characters other than 0-9 and space. If EOF
 * was reached prior to calling this, it returns 2.
 */
int TxtWaveReader::get_vector (std::vector<int> *array) {

    // If EOF is 1, we're at the end of the file and should return NULL.
    if (txtFile.eof()) {
        return 2;
    }

    txtFile.getline (line, maxWaveLen);

    // Makes sure an error has not occured during the last read.
    if (txtFile.bad() || txtFile.fail()) {
        //TODO: push_back 'NO DATA'?
        return 1;
    }

    // Makes sure it is a sequence of digits 0-9 and space.
    bool valid = is_line_valid();
    if (!valid) {
        //TODO: push_back 'NO DATA'?
        return 1;
    }

    char *pos = strtok (line, " ");
    while (pos != NULL) {
        std::string str(pos);
        array->push_back(std::stoi(str, NULL));
        pos = strtok(0, " ");
    }
    return 0;
}

/**
 * Reads next PulseData object from text file.
 * @return Returns pointer to a new PulseWave object. It will contain NO_DATA if
 * any errors occured, and will be set to NULL if EOF was reached.
 * An incomplete read will be thrown away and NULL will still be returned.
 */
PulseData* TxtWaveReader::next_wave () {
    std::vector<int> *idxArray;
    std::vector<int> *waveArray;
    PulseData *wave;

    idxArray = new std::vector<int>;
    waveArray = new std::vector<int>;
    wave = new PulseData;

    idxArray->clear();
    waveArray->clear();

    int status = get_vector (idxArray);
    if (status == 1) {
        //TODO: log error
        //TODO: Denote vector as having 'NO DATA'
    } else if (status == 2) {
        //TODO: log error
        return NULL;
    }

    status = get_vector (waveArray);
    if (status == 1) {
        //TODO: log error
        //TODO: Denote vector as having 'NO DATA'
    } else if (status == 2) {
        //TODO: log error
        return NULL;
    }

    wave->setReturning(idxArray, waveArray);
    return wave;
}

TxtWaveReader::~TxtWaveReader () {
    txtFile.close();
    free(line);
}

bool TxtWaveReader::good() {
    return txtFile.good();
}

bool TxtWaveReader::bad() {
    return txtFile.bad();
}

bool TxtWaveReader::fail() {
    return txtFile.fail();
}

bool TxtWaveReader::eof() {
    return txtFile.eof();
}
