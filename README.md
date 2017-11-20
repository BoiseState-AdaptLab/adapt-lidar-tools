# lidarDataProcessing
Contains code/project notes/ and Data for GEO+CS lidar data processing

### Build instructions
* After cloning the repo, from the root of the repo, initialize and update the **gtest** and **PulseWaves** submodules
```
git submodule init
git submodule update
```
* From the `lidarFullW_Alpha/deps/PulseWaves/src` folder, edit `pulsefilter.cpp` and `pulsetransform.cpp` to replace any instances of `atoill` with `atoll`.
* From the same folder, run make `libpulsewaves.a`. If the make is succesfull, you're good to go. 

### To run
From `lidarDataProcessing/lidarFullW_Alpha/`,

* To make and run unit tests: `make test`
* To make the lidarDriver: `make lidarDriver`
