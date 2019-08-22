#ifndef WAVETXTREADER_H
#define WAVETXTREADER_H

#include <fstream>

#include "PulseData.hpp"

class TxtWaveReader {
    public:
        TxtWaveReader();
        TxtWaveReader(int);
        int open_file(const char* filename);
        PulseData* next_wave();
        ~TxtWaveReader();

        bool good();
        bool bad();
        bool fail();
        bool eof();

    private:
        int maxWaveLen;
        std::ifstream txtFile;
        char *line;

        bool is_line_valid();
        int get_vector(std::vector<int>*);
};

#endif /* WAVETXTREADER_H */
