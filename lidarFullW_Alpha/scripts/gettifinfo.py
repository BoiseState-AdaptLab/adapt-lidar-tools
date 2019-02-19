#File name: gettifinfo.py
#Created on: 17-December-2018
#Author: Aaron Orenstein

from osgeo import gdal
from PIL import Image
import numpy as np
import math
import sys
import struct
import os
gdal.UseExceptions();

#Aaray of inputed file names
input_files = []

#Name of current file
file_name = ""

#Path to output file
custom_path = ""

def main(band_num, input_file):
  #Make sure input file is .tif
  if input_file == "-i":
    Finish(0)
  last_slash = input_file.rfind("/") + 1;
  global file_name
  #Cut down string to just filename.tif
  file_name = input_file[last_slash:]
  print ("\n\33[32mProcessing:\33[0m {}\n".format(file_name))
  if not input_file.endswith(".tif"):
    print ("Invalid file\nFile types supported: .tif")
    Finish(1)        

  #Obtain raster
  raster = gdal.Open( input_file )
  if raster is None:
    print ('Unable to open %s' % input_file);
    Finish(1)
  
  #Get raster band
  try:
    band = raster.GetRasterBand(band_num)
  except (RuntimeError, e):
    print ('No band %i found' % band_num);
    print (e);
    Finish(1)
  
  #Print out band info if requested
  if band.GetNoDataValue() == None:
    band.SetNoDataValue(-99999)
  if "-i" in sys.argv:
    print ("[ NO DATA VALUE ] = ", band.GetNoDataValue());
    print ("[ MIN ] = ", band.GetMinimum());
    print ("[ MAX ] = ", band.GetMaximum());
    print ("[ SCALE ] = ", band.GetScale());
    print ("[ UNIT TYPE ] = ", band.GetUnitType());
  ctable = band.GetColorTable()

  #Check for color table
  if ctable is None:
    print ('No ColorTable found');
  else:
    print ("[ COLOR TABLE COUNT ] = ", ctable.GetCount());
    for i in range( 0, ctable.GetCount() ):
      entry = ctable.GetColorEntry( i )
      if not entry:
        continue;
      print ("[ COLOR ENTRY RGB ] = ", ctable.GetColorEntryAsRGB( i, entry ));
  
  #Get the data
  Data(band);

def Data(band):
  #Go through each data point in the band
  #Print data to file
  print ("\nWriting data to file")
  #Get the current file name and path for output
  global file_name
  global custom_path
  #Create output file
  output = open(custom_path + file_name[:-4] + ".out", 'w')
  output.write("Max Y = {}\n\n".format(band.YSize - 1))
  #Get no data value
  no_data = band.GetNoDataValue()
  #Store max and min values and max length at all y values
  max_length, max_val, min_val = 0, no_data, no_data
  #Data will be stored here
  data = []
  #Go through each y value
  for i in range(band.YSize):
    #Read the raster
    scanline = band.ReadRaster(xoff=0, yoff=i,
                               xsize=band.XSize, ysize=1,
                               buf_xsize=band.XSize, buf_ysize=1,
                               buf_type=gdal.GDT_Float32)
    #Get data for all x values
    tuple_of_floats = struct.unpack('f' * band.XSize, scanline)
    output.write("y = {}: ".format(i))
    #Check if number of x values is bigger than previously seen
    if len(tuple_of_floats) - 1 > max_length:
      max_length = len(tuple_of_floats) - 1
    data.append([])
    #Go through every x value
    for idx, val in enumerate(tuple_of_floats):
      #Check if it's the last x value
      last = idx == len(tuple_of_floats) - 1
      #Make sure the data is not the no data value
      if not (val > no_data - 1 and val < no_data + 1):
        #Write data to output file and add it do the data list
        output.write(str(val) + ("" if last else ", "))
        data[i].append(val)
        #Check for max or min value
        if val > max_val or max_val == no_data:
          max_val = val
        if val < min_val or min_val == no_data:
          min_val = val
      else:
        #Write 'NA' for no data and indicate it in data list
        output.write("NA" + ("" if last else ", "))
        data[i].append(no_data)
    output.write("\n\n") 
  output.close()
  #Create Image
  print ("Creating Image\n")
  #Width is the size of the largest x value found
  #Height is the number of y values
  w, h = max_length + 1, band.YSize
  #Create an array that store RGB values for each data point
  #PIL conventions index colors for an image as [y,x]
  color_data = np.zeros((h, w, 3), dtype=np.uint8)
  #These are the colors for each coloring 'tier'
  #(low) dark green, yellow, red (high)
  colors = [[0,128,0],[255,255,0],[255,0,0]]
  num_colors = len(colors) - 1
  #Loop through each x value for each y value
  for idx1, vals in enumerate(data):
    for idx2, val in enumerate(vals):
      #Normalize value between 0 and 1
      val_frac = (val - min_val) / (max_val - min_val)
      #Set color tiers
      tier1, tier2 = 0, 0
      #This is the distance between value and lower tier
      between_frac = 0
      if val_frac >= 0:
        #If the value is 0 or lower, keep at lowest tier
        if val_frac >= 1:
          #If the value is 1 or higher, set to highest tier
          tier1, tier2 = num_colors, num_colors
        else:
          #Find low tier and high tier
          tier1 = math.floor(val_frac * num_colors)
          tier2 = tier1 + 1
          #Find distance between the value and the low tier (0-1)
          between_frac = (val_frac * num_colors) - tier1
      #Create color
      red = (colors[tier2][0] - colors[tier1][0]) * between_frac + colors[tier1][0]
      green = (colors[tier2][1] - colors[tier1][1]) * between_frac + colors[tier1][1]
      blue = (colors[tier2][2] - colors[tier1][2]) * between_frac + colors[tier1][2]
      #write color value to array, PIL conventions want input coords as [y, x]
      color_data[idx1, idx2] = [red, green, blue] if val != no_data else [255, 255, 255]
  #Create the image
  im = Image.fromarray(color_data, "RGB")
  im.save(custom_path + file_name[:-4] + ".jpg")
  print ("Image Created\nSaved to {}".format(custom_path + file_name[:-4] + ".jpg"))
  Finish(0);

def Finish(err):
  input_files.pop(0)
  #Check for extra input files
  if len(input_files) > 0:
    main(1, input_files[0])
  else:
    print ("\nNo more files\n")
    sys.exit(err)

if __name__ == '__main__':
  usage = """
    Usage:
    $ python {} input-raster1 [input-raster2 ...] [i] [-p path_name]
        -i: prints extra information about the tif file
    	-p path_name: Output file destination from your current location
    """.format(sys.argv[0])

  #Check arguments
  i = 1
  while i < len(sys.argv):
    if sys.argv[i] == "-p":
      #Make sure next argument is a valid path
      i += 1
      if len(sys.argv) == i or not os.path.isdir(sys.argv[i]):
        print ("""
    [ Error ] invalid or missing path

    {}""".format(usage))
        sys.exit(1)
      else:
        custom_path = sys.argv[i]
        if custom_path[-1:] != "/":
          custom_path += "/"
    elif sys.argv[i] != '-i':
      #Store any input files
      input_files.append(sys.argv[i])
    i += 1
  if len(input_files) == 0:
    print ("""
    [ Error ] you must supply at least 1 argument:
    1) input raster

    {}""".format(usage))
    sys.exit(1)

main(1, sys.argv[1]);
