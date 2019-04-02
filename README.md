# lidarDataProcessing
Contains code/project notes/ and Data for GEO+CS PulseWaves format LiDAR data processing tools

## Introduction

The PulseWaves format is a new, open, vendor-neutral, LGPL-licensed and LAS-compatible data exchange format that is aimed at storing the entire digitized waveform instead of discrete returns(like the LAS format) in a fully geo-referenced manner. The PulseWaves format consists of two binary files: **pulse files(.pls)** that describe the emitted laser pulses via a georeferenced origin and **target point and waves files(.wvs)** that contain the actual samples of the outgoing and returning waveform shapes for the digitized sections of the emitted and received waveforms. 

This project aims at developing full waveform LiDAR processing tools based on the PulseWaves data exchange format 

### Build instructions
* After cloning the repo, from the root of the repo, initialize and update the **gtest** and **PulseWaves** submodules
```
git submodule init
git submodule update
```
* From the `lidarFullW_Alpha/deps/PulseWaves/src` folder, edit `pulsefilter.cpp` and `pulsetransform.cpp` to replace any instances of `atoill` with `atoll`.
* From the same folder, run `make libpulsewaves.a`. If the make is succesfull, you're good to go. 

### To run
From `lidarDataProcessing/lidarFullW_Alpha/`,

* To make and run unit tests: `make test`
* To make the geotiff-driver: `make geotiff-driver`
