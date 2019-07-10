# Synopsis

Contains code/project notes/ and Data for GEO+CS lidar data processing

## Introduction

The PulseWaves format is a new, open, vendor-neutral, LGPL-licensed and LAS-compatible data exchange format that is aimed at storing the entire digitized waveform instead of discrete returns(like the LAS format) in a fully geo-referenced manner. The PulseWaves format consists of two binary files: **pulse files(.pls)** that describe the emitted laser pulses via a georeferenced origin and **target point and waves files(.wvs)** that contain the actual samples of the outgoing and returning waveform shapes for the digitized sections of the emitted and received waveforms. 

This project aims at developing full waveform LiDAR processing tools based on the PulseWaves data exchange format 

## Installation

### Installation on Boise State's R2 cluster

* Clone the repository: `git clone https://github.com/BoiseState-AdaptLab/adapt-lidar-tools.git`
* After cloning the directory, 
  * Load the following modules:
    * `module load gsl/gcc/2.4`
    * `module load gdal/intel/2.2.2` this should automatically load other modules that are required
  * If the installation is on your local filesystem, make sure you have the following installed: 
    * g++ (GCC 4.8.5)
    * GSL 2.4
    * GDAL 2.2.2

### Installation on a local machine running CentOS 7 linux distribution

* GSL 2.4 from: [https://ftp.gnu.org/gnu/gsl/gsl-2.4.tar.gz](https://ftp.gnu.org/gnu/gsl/gsl-2.4.tar.gz)   
follow these install instructions modified from: [http://www.linuxfromscratch.org/blfs/view/8.2/general/gsl.html](http://www.linuxfromscratch.org/blfs/view/8.2/general/gsl.html)
```
$>./configure --prefix=/usr
$>make
$>make check
$>sudo make install

$>gsl-config --lib
```

* GDAL 2.2.2 from: [https://trac.osgeo.org/gdal/wiki/DownloadSource#a2.2.2-September2017](https://trac.osgeo.org/gdal/wiki/DownloadSource#a2.2.2-September2017)   
follow theses instructions modified from [https://trac.osgeo.org/gdal/wiki/BuildingOnUnix](https://trac.osgeo.org/gdal/wiki/BuildingOnUnix)

```
$>./configure --prefix=/usr
$>make
$>sudo make install
```   
test that gdal installed with `gdalinfo --version`

* add the library path for the linker `export LD_LIBRARY_PATH=/usr/lib:$LD_LIBRARY_PATH`
    
* Run the install script `./install.sh` to automatically download, update & build the dependencies, and make the executables.
  * For R2 users, if you had errors during the installation process you most likely have a module loaded that is causing a conflict. We recommend you remove all modules `module purge` and load only the ones required to make the executables.
* If the install script ran successfully, you can find the executables in either your choice of directory if you so chose during installation, or the `bin/` folder of the `adapt-lidar-tools` directory.

**Note:** After running the install script to make sure the dependencies are built, you can at any time regenerate the executables by running `make geotiff-driver` or `make pls-info`. To cleanup and remove all executables and object files , run `make clean`.

__The waveform samples of the pulses that are reported in the Pulse Records are stored in a separate Waves file that must be in the same directory and have the same base name as the *.pls file, but have the ending *.wvs__.


## Implemented Products
These are the properties the program can analyze currently. For each property of interest, a comma separated list of 
product numbers specifies the desired output.

### Amplitude
- Short flag -a
- Long flag --amplitude

### Elevation
- Short flag -e
- Long flag --elevation

### Width
- Short flag -w
- Long flag --width

### Rise Time
- Short flag -r
- Long flag --risetime

### Backscatter Coefficient
- Short flag -b
- Long flag --backscatter

### Arguments
Combine the property type flag above with one or more product types below to generate products

| Function | Peak Type | Product Number |
|----------|-----------|----------------|
| Max      | First     | 1              |
|          | Last      | 7              |
|          | All       | 13             |
| Min      | First     | 2              |
|          | Last      | 8              |
|          | All       | 14             |
| Mean     | First     | 3              |
|          | Last      | 9              |
|          | All       | 15             |
| Std.Dev  | First     | 4              |
|          | Last      | 10             |
|          | All       | 16             |
| Skew     | First     | 5              |
|          | Last      | 11             |
|          | All       | 17             |
| Kurtosis | First     | 6              |
|          | Last      | 12             |
|          | All       | 18             |

Sorted by Product Number

| Function | Peak Type | Product Number |
|----------|-----------|----------------|
| Max      | First     | 1              |
| Min      | First     | 2              |
| Mean     | First     | 3              |
| Std.Dev  | First     | 4              |
| Skew     | First     | 5              |
| Kurtosis | First     | 6              |
| Max      | Last      | 7              |
| Min      | Last      | 8              |
| Mean     | Last      | 9              |
| Std.Dev  | Last      | 10             |
| Skew     | Last      | 11             |
| Kurtosis | Last      | 12             |
| Max      | All       | 13             |
| Min      | All       | 14             |
| Mean     | All       | 15             |
| Std.Dev  | All       | 16             |
| Skew     | All       | 17             |
| Kurtosis | All       | 18             |

### Examples
```shell
//generate max first elevation, min first elevation, mean first elevation
bin/geotiff-driver -f etc/test-file-1.pls -e 1,2,3
```

```shell
//generate std.dev first amplitude, min last amplitude, skew all amplitude
bin/geotiff-driver -f etc/test-file-1.pls -a 4,8,17
```

```shell
//generate max all width, skew last width, min all width
bin/geotiff-driver -f etc/test-file-1.pls -w 13,11,14
```

```shell
//combine the three above commands into a single run for efficiency
bin/geotiff-driver -f etc/test-file-1.pls -w 13,11,14 -a 4,8,17 -e 1,2,3
```


## Product Definitions
### AR
raw amplitude data
### AE
Estimated amplitude using Gaussian fitting
### Full Width Half Maximum (fwhm)
Estimated pulse width
### Elevation
Height on Z axis(Elevation from mean sea level (MSL))
### Peaks
Number of peaks
### Backscatter Coefficient
The backscatter coefficient compares the emitted pulse to the returning pulse within a plane normal to the path of the emitted pulse centered at the location of the peak of the returning pulse.
### Rise time
Leading edge (Time between the activation point and maximum amplitude at the rising edge of each pulse)
### Total Energy
Total energy from the estimated wave (summation of all amplitudes from wave triggering location to the wave end). Wave triggering location is found by the location that has amplitude value of max amplitude * 1/e(1). 
