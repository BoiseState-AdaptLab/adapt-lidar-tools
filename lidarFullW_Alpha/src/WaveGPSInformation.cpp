// File name: WaveGPSInformation.cpp
// Created on: 20-June-2017
// Author: ravi

#include <iostream>
#include "WaveGPSInformation.hpp"

//Default constructor
WaveGPSInformation::WaveGPSInformation(){
    // enter default values
    x_anchor = 0;
    y_anchor = 0;
    z_anchor = 0;

    x_target = 0;
    y_target = 0;
    z_target = 0;

    dx = 0;
    dy = 0;
    dz = 0;

    x_first = 0;
    y_first = 0;
    z_first = 0;

    x_last = 0;
    y_last = 0;
    z_last = 0;

    edge = 0;
    facet = 0;
    scanDirection = 0;
    intensity = 0;

    xOffset = 0;
    yOffset = 0;
    zOffset = 0;
    xScale = 0;
    yScale = 0;
    zScale = 0;
    xReal_anchor = 0;
    yReal_anchor = 0;
    zReal_anchor = 0;
    xReal_target = 0;
    yReal_target = 0;
    zReal_target = 0;
    xReal_first = 0;
    yReal_first = 0;
    zReal_first = 0;
    xReal_last = 0;
    yReal_last = 0;
    zReal_last = 0;
    xRealMin = 0;
    yRealMin = 0;
    zRealMin = 0;
    xRealMax = 0;
    yRealMax = 0;
    zRealMax = 0;

}


/**
 * Populate all the GPS data
 * @param pReader pointer to the prepared reader
 */
void WaveGPSInformation::populateGPS(PULSEreader *pReader){

    gpsTime = pReader->pulse.get_t();

    pReader->pulse.compute_anchor_and_target_and_dir();
    x_anchor = pReader->pulse.get_anchor_x();
    y_anchor = pReader->pulse.get_anchor_y();
    z_anchor = pReader->pulse.get_anchor_z();
    x_target = pReader->pulse.get_target_x();
    y_target = pReader->pulse.get_target_y();
    z_target = pReader->pulse.get_target_z();

    dx = (x_target - x_anchor) / 1000;
    dy = (y_target - y_anchor) / 1000;
    dz = (z_target - z_anchor) / 1000;

    pReader->pulse.compute_first_and_last();
    x_first = pReader->pulse.get_first_x();
    y_first = pReader->pulse.get_first_y();
    z_first = pReader->pulse.get_first_z();

    // these are saved so that we can print them if need be
    edge = pReader->pulse.edge_of_scan_line;
    scanDirection = pReader->pulse.scan_direction;
    facet = pReader->pulse.mirror_facet,
          intensity = pReader->pulse.intensity;
}


/**
 * Displays all GPS data
 */
void WaveGPSInformation::displayGPSData(){
    std::cout << x_anchor << std::endl;
    std::cout << y_anchor << std::endl;
    std::cout << z_anchor << std::endl;
    std::cout << x_target << std::endl;
    std::cout << y_target << std::endl;
    std::cout << z_target << std::endl;
    std::cout << x_first << std::endl;
    std::cout << y_first << std::endl;
    std::cout << z_first << std::endl;
    std::cout << x_last << std::endl;
    std::cout << y_last << std::endl;
    std::cout << z_last << std::endl;
    std::cout << edge << std::endl;
    std::cout << facet << std::endl;
    std::cout << scanDirection << std::endl;
    std::cout << intensity << std::endl;
}
