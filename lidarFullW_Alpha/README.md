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

### Arguments
Combine the property type flag above with one or more product types below to generate products

| Function | Peak Type | Product Number |
|----------|-----------|----------------|
| Max      | All       | 1              |
|          | First     | 7              |
|          | Last      | 13             |
| Min      | All       | 2              |
|          | First     | 8              |
|          | Last      | 14             |
| Mean     | All       | 3              |
|          | First     | 9              |
|          | Last      | 15             |
| Std.Dev  | All       | 4              |
|          | First     | 10             |
|          | Last      | 16             |
| Skew     | All       | 5              |
|          | First     | 11             |
|          | Last      | 17             |
| Kurtosis | All       | 6              |
|          | First     | 12             |
|          | Last      | 18             |

Sorted by Product Number

| Function | Peak Type | Product Number |
|----------|-----------|----------------|
| Max      | All       | 1              |
| Min      | All       | 2              |
| Mean     | All       | 3              |
| Std.Dev  | All       | 4              |
| Skew     | All       | 5              |
| Kurtosis | All       | 6              |
| Max      | First     | 7              |
| Min      | First     | 8              |
| Mean     | First     | 9              |
| Std.Dev  | First     | 10             |
| Skew     | First     | 11             |
| Kurtosis | First     | 12             |
| Max      | Last      | 13             |
| Min      | Last      | 14             |
| Mean     | Last      | 15             |
| Std.Dev  | Last      | 16             |
| Skew     | Last      | 17             |
| Kurtosis | Last      | 18             |

### Examples
```shell
//generate max all elevation, min all elevation, mean all elevation
geotiff-driver -f etc/test-file-1.pls -e 1,2,3
```

```shell
//generate std.dev all amplitude, min first amplitude, skew last amplitude
geotiff-driver -f etc/test-file-1.pls -a 4,8,17
```

```shell
//generate max last width, skew first width, min last width
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
### Back Scatter Cross Section
### Rise time
leading edge (Number of time bins between 10% and 90% of maximum amplitude at rising edge of each pulse)
### Total Energy
Total energy from the estimated wave (summation of all amplitudes from wave triggering location to the wave end). Wave triggering location is found by the location that has amplitude value of max amplitude * 1/e(1). 
