/*
 * File name: CsvWriter.hpp
 * Created on: 17-July-2019
 * Author: Spencer
 */

#ifndef CSVWRITER_HPP_
#define CSVWRITER_HPP_

#include <vector>
#include <string>

class CsvWriter {

private:

    std::vector<std::string*> *lines; //Lines to be written

public:

    CsvWriter();

    void setLines(std::vector<std::string*> *strings);
    std::vector<std::string*>* getLines();
    void freeLines();
    int write_to_csv(std::string filename);
};

#endif /* CSVWRITER_HPP_ */
