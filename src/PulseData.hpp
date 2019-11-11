/*
 * File name: PulseData.hpp
 * Created on: 12-July-2017
 * Author: ravi
 */

#ifndef PULSEDATA_HPP_
#define PULSEDATA_HPP_

#include <iostream>
#include <vector>

struct vector {
    int *buffer;
    size_t size;
    size_t capacity;
};

void resize(struct vector* v, size_t newCapacity);
void push(struct vector* v, int element);

class PulseData{

private:
  const int MAX_BUFFER_SIZE = 240;
public:
  struct vector outgoingIdx;
  struct vector outgoingWave;
  struct vector returningIdx;
  struct vector returningWave;
  long pulse_returning_start_time;
  PulseData();
  void displayPulseData(std::ostream *outStream);
  void setOutgoing(struct vector *idxArray,
                   struct vector *waveArray);
  void setReturning(struct vector *idxArray,
                    struct vector *waveArray);
};

#endif /* PULSEDATA_HPP_ */

