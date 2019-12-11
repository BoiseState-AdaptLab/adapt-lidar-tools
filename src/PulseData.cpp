// File name: PulseData.cpp
// Created on: 12-July-2017
// Author: ravi

#include <iostream>
#include <cmath>
#include "PulseData.hpp"
#include <fstream>
#include <cstdlib>

void resize(struct vector* v, size_t newCapacity) {
    int *newBuffer = new int[newCapacity];

    int min = (int)((v->size > newCapacity) ? newCapacity : v->size);
    for (int i = 0; i < min; i++) {
        newBuffer[i] = v->buffer[i];
    }

    delete[] v->buffer;
    v->buffer = newBuffer;
}

void push(struct vector* v, int element) {
    if (v->size >= v->capacity) {
        //do something
        std::cout << "Cannot insert element" << std::endl;
    } else {
         v->buffer[v->size] = element;
         v->size++;
    }
}

//Default constructor
PulseData::PulseData(){

  outgoingIdx.size = 0;
  outgoingIdx.capacity = MAX_BUFFER_SIZE;
  outgoingIdx.buffer = new int[MAX_BUFFER_SIZE];

  outgoingWave.size = 0;
  outgoingWave.capacity = MAX_BUFFER_SIZE;
  outgoingWave.buffer = new int[MAX_BUFFER_SIZE];

  returningIdx.size = 0;
  returningIdx.capacity = MAX_BUFFER_SIZE;
  returningIdx.buffer = new int[MAX_BUFFER_SIZE];

  returningWave.size = 0;
  returningWave.capacity = MAX_BUFFER_SIZE;
  returningWave.buffer = new int[MAX_BUFFER_SIZE];

  pulse_returning_start_time = 0;
}

// Deconstructor
PulseData::~PulseData(){
  outgoingIdx.size = 0;
  outgoingIdx.capacity = 0;
  delete[] outgoingIdx.buffer;

  outgoingWave.size = 0;
  outgoingWave.capacity = 0;
  delete[] outgoingWave.buffer;

  returningIdx.size = 0;
  returningIdx.capacity = 0;
  delete[] returningIdx.buffer;

  returningWave.size = 0;
  returningWave.capacity = 0;
  delete[] returningWave.buffer;

}
/**
 *
 * @param idxArray
 * @param waveArray
 */
void PulseData::setOutgoing(struct vector *idxArray,
        struct vector *waveArray){

    outgoingIdx = *idxArray;
    outgoingWave = *waveArray;
}

/**
 *
 * @param idxArray
 * @param waveArray
 */
void PulseData::setReturning(struct vector *idxArray,
        struct vector *waveArray){
    returningIdx = *idxArray;
    returningWave = *waveArray;
}


/**
 * Displays all wave data
 * @param outStream
 */
void PulseData::displayPulseData(std::ostream *outStream){

    *outStream << "\nOutgoing:\n\n";
    *outStream << "\tindices:\n";
    *outStream << "\t";
    for(int i = 0; i<(int)outgoingIdx.size; i++){
        *outStream << outgoingIdx.buffer[i] << " ";
    }
    *outStream << "\n\tamplitudes:\n";
    *outStream << "\t";
    for(int i = 0; i<(int)outgoingWave.size; i++){
        *outStream << outgoingWave.buffer[i] << " ";
    }

    *outStream << "\n\nReturning Wave:\n\n";
    *outStream << "\tindices:\n";
    *outStream << "\t";
    for(int i = 0; i<(int)returningIdx.size; i++){
        *outStream << returningIdx.buffer[i] << " ";
    }
    *outStream << "\n\tamplitudes:\n";
    *outStream << "\t";
    for(int i = 0; i<(int)returningWave.size; i++){
        *outStream << returningWave.buffer[i] << " ";
    }
    outStream->flush();
}

