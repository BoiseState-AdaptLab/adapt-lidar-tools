## Synopsis

Contains code/project notes/ and Data for GEO+CS lidar data processing

### Note:
The waveform samples of the pulses that are reported in the Pulse Records are 
stored in a separate Waves file that must be in the same directory and have the 
same base name as the *.pls file, but have the ending *.wvs.

## Products
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
