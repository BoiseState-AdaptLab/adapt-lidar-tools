##tiftools

Contains programs to extract data from tif files in various formats


##Usage

The below two usages may be used any number of times in the same command

###Analyze a tif file

python tiftools.py -iw input-tif [-p path-name]
	-i input-tif: Creates a .jpg heatmap of the tif file's data
	-w input-tif: Creates a .out text file containing the tif file's raw data
	-p path-name: Writes all output files to the specified path from the working directory

Note: Both 'w' and 'i' are not rquired however at least one argument must be provided before the input file

###Compare two tif files

python tiftools.py -c[iw] [-iw] input-tif1 [-iw] input-tif2 [-p pathname]
	-c input-tif1 input-tif2: Compares the two tif files
	-i input-tif1 input-tif2: Creates a .jpg heatmap of the differences in the two files' data on a gradient from white through red to black
	-w input-tif1 input-tif2: Creates 1) a .out text file containing both tif files' raw data shown side by side
					  2) a .out text file containing various statistics about the comparison

	-i input-tif: Creates a .jpg heatmap of the tif file's data
	-w input-tif: Creates a .out text file containing the tif file's raw data

	-p path-name: Writes all output files to the spacified path from the working directory

Note: For each individual tif file, an argument beforehand is optional
      Providing no argument will produce no output files for the individual tif file
