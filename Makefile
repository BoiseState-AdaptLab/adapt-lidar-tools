# Synopsis:
# To build the main executable (geotiff-driver) in release mode
# 	$make
# To list all targets:
# 	$make targets or $make list
# To build and run all the tests
# 	$make test
# Clean build:
# 	$make clean
# 
# To do any of the following in debug mode, add 'BUILD=debug' to the end of the make command. EX:
#	$make test BUILD=debug
# NOTE: If you go to/from release/debug, you need to run $make clean to avoid a corrupted build

all : geotiff-driver

BUILD ?= release


BIN = bin
SRC = src
OBJ = obj
DEP = depend

PULSEWAVES = deps/PulseWaves

CXX = g++

CXXFLAGS += -std=c++11 -Wall -Wextra -Wnon-virtual-dtor -pedantic -Wcast-align -Woverloaded-virtual -Wpedantic -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2 -Wno-float-conversion

DEPFLAGS = -MT $@ -MMD -MP -MF $(DEP)/$*.d #For generating dependencies

LDFLAGS = -lpthread -lm -lgsl -lcblas -lgdal -L $(PULSEWAVES)/lib -lpulsewaves

INCFLAGS = -isystem $(PULSEWAVES)/inc -isystem deps/ -isystem deps/spdlog # @@ TODO -isystem deps/googletest/googletest/include


# Verify valid BUILD flag and adjust CXXFLAGS
ifeq ($(BUILD), release)
	CXXFLAGS += -O3 -flto
else ifeq ($(BUILD), debug)
	CXXFLAGS += -ggdb3 -D _GLIBCXX_DEBUG -D _GLIBCXX_DEBUG_PEDANTIC #STL debug version (range checked vector[] for instance)
else
$(error Invalid BUILD flag, choose either release (default) or debug)	#The lack of indentation is required
endif

#Executables
geotiff-driver: $(BIN)/geotiff-driver

$(BIN)/geotiff-driver: \
		$(OBJ)/CmdLine.o \
		$(OBJ)/Common.o \
		$(OBJ)/FlightLineData.o \
		$(OBJ)/GaussianFitter.o \
		$(OBJ)/GDALWriter.o \
		$(OBJ)/GeoTIFFConsumer.o \
		$(OBJ)/GeoTIFFDriver.o \
		$(OBJ)/GeoTIFFWrapper.o \
		$(OBJ)/LidarVolume.o \
		$(OBJ)/Peak.o \
		$(OBJ)/PeakProducts.o \
		$(OBJ)/PreProcess.o \
		$(OBJ)/PulseWavesProducer.o \
		$(OBJ)/WaveGPSInformation.o
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)


# See http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/ for more details

# @@TODO Special for gtest?
$(OBJ)/%.o : $(SRC)/%.cpp $(DEP)/%.d | $(DEP) #| $(BIN) | $(OBJ)
	$(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(INCFLAGS) $< -c -o $@

SRCS := $(notdir $(wildcard $(SRC)/*.cpp))
DEPFILES := $(SRCS:%.cpp=$(DEP)/%.d)

$(DEPFILES):
include $(wildcard $(DEPFILES))

#Make directories if DNE
$(DEP):
	@mkdir -p $@
$(BIN):
	@mkdir -p $@
$(OBJ):
	@mkdir -p $@

#Clear object files and BIN
.PHONY: clean
clean: 
	rm -f $(BIN)/*
	rm -f $(OBJ)/*

# Also remove dependency files
.PHONY: fullclean
fullclean: clean
	rm -f $(DEP)/*

