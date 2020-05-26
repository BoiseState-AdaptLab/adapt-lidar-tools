<p align="center"><img width="45%" src="https://github.com/BoiseState-AdaptLab/adapt-lidar-tools/blob/master/adapt-logo.png"></p>


### Adapt Lidar Tools is a GPL-Licensed set of C++11 Tools for Processing Full-Waveform Lidar Data
#### Developed by a [team of students](https://github.com/BoiseState-AdaptLab/adapt-lidar-tools/blob/master/pubs/AdaptLidarTools-A.Full.Waveform.Lidar.Processing.Suite.pdf)

# Outline:
 * [Introduction](#Introduction)
 * [Installation](#Installation)
   * [Installation On Boise State's R2 Cluster](#Install1)
   * [Installation On a Normal Machine](#Install2)
   * [Installation With Docker](#Install3)
 * [Implemented Products](#implemented)
   * [Amplitude](#amplitude)
   * [Elevation](#elevation)
   * [Width](#width)
   * [Rise Time](#risetime)
   * [Backscatter Coefficient](#backscatter)
   * [Arguments](#arguments)
   * [Examples](#examples)
 * [Product Definitions](#definitions)
   * [AR](#ar)
   * [AE](#ae)
   * [Full Width Half Maximum (fwhm)](#fwhm)
   * [Elevation](#elevation-def)
   * [Peaks](#peaks)
   * [Backscatter Coefficient](#backscatter-def)
   * [Rise Time](#risetime-def)
   * [Total Energy](#total-energy)
 * [Student Participants](#studentparticipants)



## Introduction

The PulseWaves format is a new, open, vendor-neutral, LGPL-licensed and LAS-compatible data exchange format that is aimed at storing the entire digitized waveform instead of discrete returns(like the LAS format) in a fully geo-referenced manner. The PulseWaves format consists of two binary files: **pulse files(.pls)** that describe the emitted laser pulses via a georeferenced origin and **target point and waves files(.wvs)** that contain the actual samples of the outgoing and returning waveform shapes for the digitized sections of the emitted and received waveforms. 

This project aims at developing full waveform LiDAR processing tools based on the PulseWaves data exchange format 

## Installation

<a name="install1"></a>
### Installation on Boise State's R2 cluster

* **Clone** the repository: `git clone https://github.com/BoiseState-AdaptLab/adapt-lidar-tools.git`
* After **cloning** the directory, 
  * Load the following modules:
    * `module load gsl/gcc/2.4`
    * `module load gdal/intel/2.2.2` this should automatically load other modules that are required
* [**Click for Continuation**](#continuation)

<a name="install2"></a>
### Installation on a Local Machine

Requirements:
* g++ (Must Support C++11)
* GSL 2.4 or newer
* GDAL 2.2.2 or newer

Directions:
* **Clone** the repository: `git clone https://github.com/BoiseState-AdaptLab/adapt-lidar-tools.git`
* [**Click for Continuation**](#continuation)

<a name="Install3"></a>
### Installation With Docker
* Follow this **readme** located on our DockerHub: [https://hub.docker.com/repository/docker/nicholasprussen/adapt-lidar-tools-docker](https://hub.docker.com/repository/docker/nicholasprussen/adapt-lidar-tools-docker)

***
<a name="continuation"></a>    
**Installation Continuation**: (Only do this after following your appropiate installation instructions)
* **Change directory** into the adapt-lidar-tools repository.
* **Run the install script** `./install.sh` to automatically download, update & build the dependencies, and make the executables.
  * **For R2 users**, if you had errors during the installation process you most likely have a module loaded that is causing a conflict. We recommend you remove all modules `module purge` and load only the ones required to make the executables.
* If the install script **ran successfully**, you can find the executables in either your choice of directory if you so chose during installation, or the `bin/` folder of the `adapt-lidar-tools` directory.

**Note:** After running the install script to make sure the dependencies are built, you can at any time regenerate the executables by running `make geotiff-driver` or `make pls-info`. To cleanup and remove all executables and object files , run `make clean`.

__The waveform samples of the pulses that are reported in the Pulse Records are stored in a separate Waves file that must be in the same directory and have the same base name as the *.pls file, but have the ending *.wvs__.


<a name="implemented"></a>
## Implemented Products
These are the properties the program can analyze currently. For each property of interest, a comma separated list of 
product numbers specifies the desired output.

<a name="amplitude"></a> 
### Amplitude
- Short flag -a
- Long flag --amplitude

<a name="elevation"></a> 
### Elevation
- Short flag -e
- Long flag --elevation

<a name="width"></a> 
### Width
- Short flag -w
- Long flag --width

<a name="risetime"></a> 
### Rise Time
- Short flag -r
- Long flag --risetime

<a name="backscatter"></a> 
### Backscatter Coefficient
- Short flag -b
- Long flag --backscatter

<a name="arguments"></a> 
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

<a name="examples"></a> 
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

<a name="definitions"></a> 
## Product Definitions
<a name="ar"></a> 
### AR
raw amplitude data
<a name="ae"></a> 
### AE
Estimated amplitude using Gaussian fitting
<a name="fwhm"></a> 
### Full Width Half Maximum (fwhm)
Estimated pulse width
<a name="elevation-def"></a> 
### Elevation
Height on Z axis(Elevation from mean sea level (MSL))
<a name="peaks"></a> 
### Peaks
Number of peaks
<a name="backscatter-def"></a> 
### Backscatter Coefficient
The backscatter coefficient compares the emitted pulse to the returning pulse within a plane normal to the path of the emitted pulse centered at the location of the peak of the returning pulse.
<a name="risetime-def"></a> 
### Rise time
Leading edge (Time between the activation point and maximum amplitude at the rising edge of each pulse)
<a name="total-energy"></a> 
### Total Energy
Total energy from the estimated wave (summation of all amplitudes from wave triggering location to the wave end). Wave triggering location is found by the location that has amplitude value of max amplitude * 1/e(1). 

<a name="studentparticipants"></a>
## Student Participants (alphabetical)

* Nicholas Chapa
* Floriana Ciaglia
* Brandon Echols
* Jason Egbert
* Spencer Fleming
* Ahmad Hojatimalekshah
* Nayani Ilangakoon
* Ariel Marvasti
* Aaron Orenstein
* Nicholas Prussen
* Ahmad Rezaii
* Ravi Shankar
* Jared White
