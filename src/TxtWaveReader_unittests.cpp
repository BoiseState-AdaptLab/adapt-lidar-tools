// File name: TxtWaveReader_unittests.cpp
// Created on: 18 Aug 2019
// Author: Spencer Fleming

#include <fstream>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>

#include "gtest/gtest.h"
#include "TxtWaveReader.hpp"

#include "PulseData.hpp"

#define DEFAULT_FILENAME "testfile.txt"
#define FILENAME_MAXSIZE 100

class TxtWaveReaderTest: public testing::Test {
    public:
        char* fileName;
        std::string message;
        TxtWaveReader reader;

        void writeTestFile () {
            std::ofstream ofs (fileName);
            ofs << message;
            ofs.close();
        }

        // Useful tool for debugging
        void printVector (std::vector<int>& vect) {
            std::string str = "";

            for (std::vector<int>::iterator it = vect.begin(); it != vect.end();
                    ++it) {
                str += *it;
                str += " ";
            }

            std::cout << str << std::endl;
        }

        virtual void SetUp() {
            fileName = (char*) malloc(sizeof(char) * FILENAME_MAXSIZE);
            std::strncpy(fileName, DEFAULT_FILENAME, FILENAME_MAXSIZE);
        }

        virtual void TearDown() {
            std::remove(fileName);
            free(fileName);
        }
};

//******************************************************************************
//* Tests the writeTestFile() to make sure it's reliable for testing
//******************************************************************************

// Proves that writing to file works.
TEST_F(TxtWaveReaderTest, writeTestFileTest) {
    std::ifstream file;
    int msg_size = 16;
    char reading[msg_size];

    message = "Message to read";
    writeTestFile();

    EXPECT_NO_THROW(file.open(fileName));
    EXPECT_TRUE(file.good());
    EXPECT_NO_THROW(file.read(reading, msg_size));
    EXPECT_STREQ(reading, message.c_str());
}

// Proves that after the last test the txt file is gone.
// Does so by allowing an open to fail.
TEST_F(TxtWaveReaderTest, tmpFileGetsDeletedTest) {
    std::ifstream file;

    file.open(fileName);
    EXPECT_TRUE(file.fail());
}

//******************************************************************************
//* Entire process tests, reads file in and everything.
//******************************************************************************

// Test proves blank files are handled properly
TEST_F(TxtWaveReaderTest, blankFileTest) {
    writeTestFile();

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_FALSE(reader.next_wave());
    EXPECT_TRUE(reader.idx.empty());
}

// One token: Reader expected to open fine and return false when wave is read.
TEST_F(TxtWaveReaderTest, oneIntTest) {
    message = "9";
    writeTestFile();

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_FALSE(reader.next_wave());
}

// One line: Reader is expected to open fine and return false when wave is read.
TEST_F(TxtWaveReaderTest, oneValidLineTest) {
    message = "9 8 7 6 5 4 3 2 1 0";

    // These are the expected vectors. In this case, waves technically undefined
    std::vector<int> exp_idx = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
    std::vector<int> exp_wave = { };
    writeTestFile();

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_FALSE(reader.next_wave());

    //printVector(reader.idx);
    //printVector(reader.wave);

    EXPECT_TRUE(reader.idx == exp_idx);
    EXPECT_TRUE(reader.wave == exp_wave);
}

// Proves that file containing a single valid wave will process correctly
TEST_F(TxtWaveReaderTest, oneValidWaveTest) {
    message = "9 8 7 6 5 4\n3 2 1 0";

    std::vector<int> exp_idx = { 9, 8, 7, 6, 5, 4 };
    std::vector<int> exp_wave = { 3, 2, 1, 0 };
    writeTestFile();

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_TRUE(reader.next_wave());

    //printVector(reader.idx);
    //printVector(reader.wave);

    EXPECT_TRUE(reader.idx == exp_idx);
    EXPECT_TRUE(reader.wave == exp_wave);
    EXPECT_FALSE(reader.next_wave());
}

// Proves that incorrect characters present in pulsewave are handled properly.
TEST_F(TxtWaveReaderTest, oneInvalidWaveTest) {
    message = "9 8 7 6 5 4\n3 bahbah 4";
    writeTestFile();

    std::vector<int> idx0 = { 9, 8, 7, 6, 5, 4 };
    std::vector<int> wave0 = { };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_TRUE(reader.next_wave());

    //printVector(reader.idx);
    //printVector(reader.wave);

    EXPECT_TRUE(reader.idx == idx0);
    EXPECT_TRUE(reader.wave == wave0);
    EXPECT_FALSE(reader.next_wave());
}

TEST_F(TxtWaveReaderTest, threeValidLineTest) {
    message = "9 8 7\n6 5 3\n2 1 0";
    writeTestFile();

    std::vector<int> idx0 = { 9, 8, 7 };
    std::vector<int> wave0= { 6, 5, 3 };
    std::vector<int> idx1 = { 2, 1, 0 };
    std::vector<int> wave1 = { };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_TRUE(reader.next_wave());
    EXPECT_TRUE(reader.idx == idx0);
    EXPECT_TRUE(reader.wave == wave0);

    EXPECT_FALSE(reader.next_wave());
    EXPECT_TRUE(reader.idx == idx1);
    EXPECT_TRUE(reader.wave == wave1);
}

TEST_F(TxtWaveReaderTest, twoValidWaveTest) {
    message = "9 8 7\n6 5 4\n3 2 1\n0 1 2";
    writeTestFile();

    std::vector<int> idx0 = { 9, 8, 7 };
    std::vector<int> wave0 = { 6, 5, 4 };
    std::vector<int> idx1 = { 3, 2, 1 };
    std::vector<int> wave1 = { 0, 1, 2 };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_TRUE(reader.next_wave());

    //printVector(reader.idx);
    //printVector(reader.wave);

    EXPECT_TRUE(reader.idx == idx0);
    EXPECT_TRUE(reader.wave == wave0);
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx1);
    EXPECT_TRUE(reader.wave == wave1);
    EXPECT_FALSE(reader.next_wave());
}

TEST_F(TxtWaveReaderTest, Valid_Invalid_ValidWaveTest) {
    message = "9 8 7\n6 5 4\n3 2 1\n0 bah 2\n3 4 5\n6 7 8";
    writeTestFile();

    std::vector<int> idx0 = { 9, 8, 7 };
    std::vector<int> wave0 = { 6, 5, 4 };
    std::vector<int> idx1 = { 3, 2, 1 };
    std::vector<int> wave1 = { };
    std::vector<int> idx2 = { 3, 4, 5 };
    std::vector<int> wave2 = { 6, 7, 8 };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx0);
    EXPECT_TRUE(reader.wave == wave0);
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx1);
    EXPECT_TRUE(reader.wave == wave1);
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx2);
    EXPECT_TRUE(reader.wave == wave2);
    EXPECT_FALSE(reader.next_wave());
}

TEST_F(TxtWaveReaderTest, Valid_Invalid_readanyways_Test) {
    message = "9 8 7 6 5 4\n9 8 7 6 5 4\n3 2 1\n";
    writeTestFile();

    std::vector<int> idx0 = { 9, 8, 7, 6, 5, 4};
    std::vector<int> wave0 = { 9, 8, 7, 6, 5, 4};
    std::vector<int> idx1 = { 3, 2, 1 };
    std::vector<int> wave1 = { };
    std::vector<int> idx2 = { };
    std::vector<int> wave2 = { };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx0);
    EXPECT_TRUE(reader.wave == wave0);
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx1);
    EXPECT_TRUE(reader.wave == wave1);
    EXPECT_FALSE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx2);
    EXPECT_TRUE(reader.wave == wave2);
    EXPECT_FALSE(reader.next_wave());
}

TEST_F(TxtWaveReaderTest, Valid_Valid_EmptyAndReadsAnyways_Test) {
    message = "1 2 3\n1 2 3\n4 5 6\n4 5 6\n\n";
    writeTestFile();

    std::vector<int> idx0 = { 1, 2, 3 };
    std::vector<int> wave0 = { 1, 2, 3 };
    std::vector<int> idx1 = { 4, 5, 6 };
    std::vector<int> wave1 = { 4, 5, 6 };
    std::vector<int> idx2 = { };
    std::vector<int> wave2 = { };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx0);
    EXPECT_TRUE(reader.wave == wave0);
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx1);
    EXPECT_TRUE(reader.wave == wave1);
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx2);
    EXPECT_TRUE(reader.wave == wave2);
    EXPECT_FALSE(reader.next_wave());
}

TEST_F(TxtWaveReaderTest, Valid_Valid_EOF_andReadsAnyways_Test) {
    message = "1 1 1\n1 1 1\n2 2 2\n 2 2 2\n";
    writeTestFile();

    std::vector<int> idx0 = { 1, 1, 1 };
    std::vector<int> wave0 = { 1, 1, 1 };
    std::vector<int> idx1 = { 2, 2, 2 };
    std::vector<int> wave1 = { 2, 2, 2 };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx0);
    EXPECT_TRUE(reader.wave == wave0);
    EXPECT_TRUE(reader.next_wave());

    EXPECT_TRUE(reader.idx == idx1);
    EXPECT_TRUE(reader.wave == wave1);
    EXPECT_FALSE(reader.next_wave());
}
