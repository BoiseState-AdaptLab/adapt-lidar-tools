## Synopsis

Contains code/project notes/ and Data for GEO+CS lidar data processing

### Note:
The waveform samples of the pulses that are reported in the Pulse Records are 
stored in a separate Waves file that must be in the same directory and have the 
same base name as the *.pls file, but have the ending *.wvs.

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
geotiff-driver -f etc/test-file-1.pls -e 1,2,3
```

```shell
//generate std.dev first amplitude, min last amplitude, skew all amplitude
geotiff-driver -f etc/test-file-1.pls -a 4,8,17
```

```shell
//generate max all width, skew last width, min all width
geotiff-driver -f etc/test-file-1.pls -w 13,11,14
```

```shell
//combine the three above commands into a single run for efficiency
geotiff-driver -f etc/test-file-1.pls -w 13,11,14 -a 4,8,17 -e 1,2,3
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
