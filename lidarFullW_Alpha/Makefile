CPP=g++
CFLAGS=-I.
SRC=src
BIN=bin

lidarDriver: $(SRC)/lidarDriver.o $(SRC)/cmdLine.o
	$(CPP) -o $(BIN)/$@ $^ $(CFLAGS)  

test: $(SRC)/testDriver.o $(SRC)/cmdLine.o
	$(CPP) -o $(BIN)/$@ $^ $(CFLAGS)  
	bin/test


