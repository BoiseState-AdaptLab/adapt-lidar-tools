# tiftools

Contains a program to extract data from tif files in various formats

## Usage

The below usages may be used any number of times in the same command as long as they are given the correct number of arguments

### Analyze a tif file

<pre>
python tiftools.py -iw input-tif [-p path-name]  
    -i input-tif: Creates a .jpg heatmap of the tif file's data
    -w input-tif: Creates a .out text file containing the tif file's raw data
    -p path-name: Writes all output files to the specified path from the working directory

<b>Note: It is not required to have both 'w' and 'i', however at least one must be provided before the input file</b>
</pre>

### Compare two tif files

<pre>
python tiftools.py -ciw [-iw] input-tif1 [-iw] input-tif2 [-p pathname]
    -c input-tif1 input-tif2: Compares the two tif files
    -i input-tif1 input-tif2: Creates a .jpg heatmap of the differences in the two files' data on a gradient
    -w input-tif1 input-tif2: Creates 1) a .out text file containing both tif files' raw data shown side by side  
				      2) a .out text file containing various statistics about the comparison

    -i input-tif: Creates a .jpg heatmap of the tif file's data
    -w input-tif: Creates a .out text file containing the tif file's raw data

    -p path-name: Writes all output files to the spacified path from the working directory

<b>Note: For the comparison heatmap, green and blue denote that one tif file had data while the other did not  
      For the gradient, white represents small differences while black represents large differences</b>
</pre>
