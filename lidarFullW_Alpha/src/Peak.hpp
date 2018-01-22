/*
 * File name: Peak.hpp
 * Created on: 15-December-2017
 * Author: olschanowsky
 */

#ifndef PEAK_HPP_
#define PEAK_HPP_

/*struct peak{
	  double time;
	  double amp;
	  double width;
};

struct peaks{
	  int count;
	  struct peak* peakList;
};*/

double amp;
double location;    //The time location of peak occurence
double fwhm;        //Full width half maximum - the width of the peak

int peak_position;  //Given 'n' peaks, the position of this peak

int peak_triggering_location; //Peak activation time - one ahead of the noise

//(peak_triggering_location - t_first) * dx + x_first
int peak_x_activation;

//(peak_triggering_location - t_first) * dy + y_first
int peak_y_activation;

//(peak_triggering_location - t_first) * dz + z_first
int peak_z_activation;

#endif /* PEAK_HPP_ */
