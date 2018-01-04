/*
 * File name: Peak.hpp
 * Created on: 15-December-2017
 * Author: olschanowsky
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
