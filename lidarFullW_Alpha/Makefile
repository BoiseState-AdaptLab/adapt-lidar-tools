CPP=g++
CFLAGS=-I.
SRC=src
BIN=bin

lidarDriver: $(SRC)/lidarDriver.o $(SRC)/cmdLine.o $(SRC)/parseFile.o
	$(CPP) -o $(BIN)/$@ $^ $(CFLAGS)  

test: $(SRC)/testDriver.o $(SRC)/cmdLine.o $(SRC)/parseFile.o
	$(CPP) -o $(BIN)/$@ $^ $(CFLAGS)  
	bin/test

clean: rm $(BIN)/* $(SRC)/*.o
	bin/rm *
	src/rm *.o


