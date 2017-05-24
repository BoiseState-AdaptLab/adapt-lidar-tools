CPP=g++
CFLAGS=-I.
SRC=src
BIN=bin

# Points to the root of Google Test, relative to where this file is.
GTEST_DIR = ../../googletest/googletest

# All Google Test headers.
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

lidarDriver: $(SRC)/lidarDriver.o $(SRC)/cmdLine.o $(SRC)/parseFile.o
	$(CPP) -o $(BIN)/$@ $^ $(CFLAGS)  

test: $(SRC)/testDriver.o $(SRC)/cmdLine.o $(SRC)/parseFile.o
	$(CPP) -o $(BIN)/$@ $^ $(CFLAGS)  
	bin/test

clean:
	rm bin/*
	rm src/*.o


