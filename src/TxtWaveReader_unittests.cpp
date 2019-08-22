// File name: TxtWaveReader_unittests.cpp
// Created on: 18 Aug 2019
// Author: Spencer Fleming

#include <fstream>
#include <cstring>
#include <string>
#include <iostream>

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

        std::vector<PulseData*> data;

        void writeTestFile () {
            std::ofstream ofs (fileName);
            ofs << message;
            ofs.close();
        }

        virtual void SetUp() {
            fileName = (char*) malloc(sizeof(char) * FILENAME_MAXSIZE);
            std::strncpy(fileName, DEFAULT_FILENAME, FILENAME_MAXSIZE);
        }

        virtual void TearDown() {
            std::remove(fileName);
            free(fileName);
            for (auto it = data.begin(); it != data.end(); ++it) {
                delete(*it);
            }
            data.clear();
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
    message = "";
    writeTestFile();

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));

    EXPECT_TRUE(data[0] == NULL);
}

// This and next test prove files without enough information to make a PulseData
// object are handled properly.
TEST_F(TxtWaveReaderTest, oneIntTest) {
    message = "9";
    writeTestFile();

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));

    EXPECT_TRUE(data[0] == NULL);
}

TEST_F(TxtWaveReaderTest, oneValidLineTest) {
    message = "9 8 7 6 5 4 3 2 1 0";
    writeTestFile();

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));

    EXPECT_TRUE(data[0] == NULL);
}

// Proves that file containing a single valid wave will process correctly and
// produce accurate PulseData objects.
TEST_F(TxtWaveReaderTest, oneValidWaveTest) {
    message = "9 8 7 6 5 4\n3 2 1 0";

    // These are what PulseData vectors should look like after parsing
    std::vector<int> idx = { 9, 8, 7, 6, 5, 4 };
    std::vector<int> wave = { 3, 2, 1, 0 };
    writeTestFile();

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));

    ASSERT_FALSE(data[0] == NULL);
    EXPECT_TRUE(data[0]->returningIdx == idx);
    EXPECT_TRUE(data[0]->returningWave == wave);
}

// Proves that incorrect characters present in pulsewave are handled properly.
TEST_F(TxtWaveReaderTest, oneInvalidWaveTest) {
    message = "9 8 7 6 5 4\n3 bahbah 4";
    writeTestFile();

    std::vector<int> idx = { 9, 8, 7, 6, 5, 4 };
    std::vector<int> wave = { };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));

    ASSERT_FALSE(data[0] == NULL);
    EXPECT_TRUE(data[0]->returningIdx == idx);
    EXPECT_TRUE(data[0]->returningWave == wave);
}

// Test proves that if file has odd number of lines, the last line is ignored.
TEST_F(TxtWaveReaderTest, threeValidLineTest) {
    message = "9 8 7\n6 5 3\n2 1 0";
    writeTestFile();

    std::vector<int> idx = { 9, 8, 7 };
    std::vector<int> wave = { 6, 5, 3 };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));

    ASSERT_FALSE(data[0] == NULL);
    EXPECT_TRUE(data[1] == NULL);

    EXPECT_TRUE(data[0]->returningIdx == idx);
    EXPECT_TRUE(data[0]->returningWave == wave);
}

TEST_F(TxtWaveReaderTest, twoValidWaveTest) {
    message = "9 8 7\n6 5 4\n3 2 1\n0 1 2";
    writeTestFile();

    std::vector<int> idx0 = { 9, 8, 7 };
    std::vector<int> wave0 = { 6, 5, 4 };
    std::vector<int> idx1 = { 3, 2, 1 };
    std::vector<int> wave1 = { 0, 1, 2 };

    EXPECT_NO_THROW(reader.open_file (fileName));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));

    ASSERT_FALSE(data[0] == NULL);
    ASSERT_FALSE(data[1] == NULL);

    EXPECT_TRUE(data[0]->returningIdx == idx0);
    EXPECT_TRUE(data[0]->returningWave == wave0);
    EXPECT_TRUE(data[1]->returningIdx == idx1);
    EXPECT_TRUE(data[1]->returningWave == wave1);
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
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));
    EXPECT_NO_THROW(data.push_back(reader.next_wave()));

    ASSERT_FALSE(data[0] == NULL);
    ASSERT_FALSE(data[1] == NULL);
    ASSERT_FALSE(data[2] == NULL);

    EXPECT_TRUE(data[0]->returningIdx == idx0);
    EXPECT_TRUE(data[0]->returningWave == wave0);
    EXPECT_TRUE(data[1]->returningIdx == idx1);
    EXPECT_TRUE(data[1]->returningWave == wave1);
    EXPECT_TRUE(data[2]->returningIdx == idx2);
    EXPECT_TRUE(data[2]->returningWave == wave2);
}
