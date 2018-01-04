/*
 * File name: Peak.hpp
 * Created on: 13-October-2017
 * Author: ravi
 * Modified by: olschanowsky
 */

#ifndef PEAK_HPP_
#define PEAK_HPP_

struct peak{
  double time;
  double amp;
  double width;
};

struct peaks{
  int count;
  struct peak* peakList;
};

#endif /* PEAK_HPP_ */
