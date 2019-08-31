#ifndef WAVETXTREADER_H
#define WAVETXTREADER_H

#include <vector>
#include <fstream>

class TxtWaveReader {
    public:
        TxtWaveReader();
        TxtWaveReader(int maxWaveLen);
        int open_file(const char* filename);
        bool next_wave();
        ~TxtWaveReader();

        std::vector<int> idx;
        std::vector<int> wave;

    private:
        int maxWaveLen;
        std::ifstream txtFile;
        char *line;

        bool is_line_valid();
        bool get_vector(std::vector<int>& vect);
        int line_count;
};

#endif /* WAVETXTREADER_H */
