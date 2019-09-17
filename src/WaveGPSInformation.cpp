// File name: WaveGPSInformation.cpp
// Created on: 20-June-2017
// Author: ravi

#include <iostream>
#include "WaveGPSInformation.hpp"
#include <iomanip>
#include "spdlog/spdlog.h"

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
    x_last = pReader->pulse.get_last_x();
    y_last = pReader->pulse.get_last_y();
    z_last = pReader->pulse.get_last_z();

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
    spdlog::trace("{}", x_anchor);
    spdlog::trace("{}", y_anchor);
    spdlog::trace("{}", z_anchor);
    spdlog::trace("{}", x_target);
    spdlog::trace("{}", y_target);
    spdlog::trace("{}", z_target);
    spdlog::trace("{}", x_first);
    spdlog::trace("{}", y_first);
    spdlog::trace("{}", z_first);
    spdlog::trace("{}", x_last);
    spdlog::trace("{}", y_last);
    spdlog::trace("{}", z_last);
    spdlog::trace("{}", edge);
    spdlog::trace("{}", facet);
    spdlog::trace("{}", scanDirection);
    spdlog::trace("{}", intensity);

}
