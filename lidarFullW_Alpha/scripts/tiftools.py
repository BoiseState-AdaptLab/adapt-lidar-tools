#File name: analyzetif.py
#Created on: 17-December-2018
#Author: Aaron Orenstein

from osgeo import gdal
from PIL import Image
import numpy as np
import math
import sys
import struct
import os
import time
import datetime
from pathlib import Path
from tif import Tif
gdal.UseExceptions()

def main(arg_set, path):
  #For debugging
  #print (arg_set)
  global args
  #Check if user called a comparison or not
  if 'c' not in arg_set[0]:
    #Create Tif object and check if we are writing an ouput
    #and/or creating an image
    tif = Tif(arg_set[1])
    if 'w' in arg_set[0]:
      writeData(tif, path)
    if 'i' in arg_set[0]:
      writeImage(tif, path)
    #If arg_set not in args, then it is part of a comparison so return tif
    if arg_set not in args:
      return tif
  else:
    print ("\33[32mBeginning comparison\33[0m\n")
    #Create tif objects to compare
    tifA = main([arg_set[1], arg_set[2]], path)
    tifB = main([arg_set[3], arg_set[4]], path)
    #Make sure tif files are of equal length
    ans = "y"
    if tifA.maxY != tifB.maxY:
      print ("[ Error ] tif files are not of equal length")
      print ("File A has a max Y of {}. File B has a max Y of {}\n"
             .format(tifA.maxY, tifB.maxY))
      ans = ""
      while ans != "y" and ans != "n":
        ans = input("Continue comparison? (y/n) ")
    if ans is "y":
      #Find unused comparison number
      j = 1
      while (Path(path + "compare_{}_simple.out".format(j)).is_file() or
             Path(path + "compare_{}_raw.out".format(j)).is_file() or
             Path(path + "compare_{}.jpg".format(j)).is_file()):
        j += 1
      #Check if we are writing an output and/or creating an image
      if 'w' in arg_set[0]:
        compareData(tifA, tifB, path, j)
      if 'i' in arg_set[0]:
        writeCompareImg(tifA, tifB, path, j)
    else:
      print("Skipping...")  

  #Get rid of this arg_set and move on to the next arg_set
  args.pop(0)
  if (len(args) > 0):
    main(args[0], path)

def writeData(tif, path):
  #Go through each data point in the band
  #Print data to file
  print ("Writing data to file\n")
  #Get data
  data = tif.data
  #Create output file
  output = open(path + tif.file_name[:-4] + ".out", 'w')
  output.write("Max Y = {}\n\n".format(tif.maxY))
  
  #Go through each y value
  for y, vals in enumerate(data):
    output.write("y = {}: ".format(y))
    for x, val in enumerate(vals):
      #Check if it's the last x value
      last = x == len(vals) - 1
      #Make sure the data is not the no data value
      if val != tif.no_value:
        #Write data to output file and add it do the data list
        output.write(str(val) + ("" if last else ", "))
      else:
        #Write 'NA' for no data and indicate it in data list
        output.write("NA" + ("" if last else ", "))
    output.write("\n\n") 
  output.close()
  print ("Data written to {}\n".format(path + tif.file_name[:-4] + ".out"))

#Writes image data to a jpg
def writeImage(tif, path):
  #Get color data
  color_data = tif.getImgData()
  img = Image.fromarray(color_data, 'RGB')
  img.save(path + tif.file_name[:-4] + '.jpg')
  print("Image written to {}\n".format(path + tif.file_name[:-4] + '.jpg'))

def compareData(tifA, tifB, path, compare_no):
  print ("Writing comparison to file\n")
  A = 0
  B = 1
  #Get smallest maxY
  maxY = min(tifA.maxY, tifB.maxY)
  #Create file to write to
  #'Simple' has the totals for each comparison result
  #'Raw' has the comparison for every data point
  outputs = [open(path + "compare_{}_simple.out".format(compare_no), 'w'),
             open(path + "compare_{}_raw.out".format(compare_no), 'w')]
  #Write the date, time, and input file names on the top of the output files
  for out in outputs:
    out.write("Created at {}\n".format(datetime.datetime.fromtimestamp(time.time()).
              strftime('%Y-%m-%d %H:%M:%S')))
    out.write("file A is {}, file B is {}\n".format(tifA.file_name, tifB.file_name))
    out.write("Max Y = {}\n".format(maxY))
  outputs[0].write("The simple file contains the totals for each comparison result for each y-value\n\n")
  outputs[1].write("The raw file contains the comparison result for each data point\n\n")

  #Begin comparison
  #Cycle through the data for each y value
  for y in range(maxY + 1):
    #Write the current y value to the 'raw' file
    outputs[0].write("y = {}: ".format(y))
    outputs[1].write("y = {}: ".format(y))
    #0 = both no data, 1 = A has data, 2 = B has data, 3 = both have data
    comparison = [0,0,0,0]
    #This holds the differences in values
    differences = []
    first = True

    #Compare each data point and write the result to the 'raw' file
    for x in range(min([tifA.maxX, tifB.maxX])):
      #Get data points and no values
      a, b = tifA.data[y,x], tifB.data[y,x]
      nvA, nvB = tifA.no_value, tifB.no_value
      #', ' is added before the next data comparison
      #This way is will not be added after the last data point
      #However we don't want it before the first data point
      if not first:
        outputs[1].write(", ")
      if a != nvA and b != nvB:
        #Both
        outputs[1].write("[{}|{}]".format(a, b))
        comparison[3] += 1
        differences.append(float(a) - float(b))
      elif a != nvA and b == nvB:
        #Just A
        outputs[1].write("[{}|NA]".format(a))
        comparison[1] += 1
      elif a == nvA and b != nvB:
        #Just B
        outputs[1].write("[NA|{}]".format(b))
        comparison[2] += 1
      else:
        #Neither
        outputs[1].write("NA")
        comparison[0] += 1
      first = False

    outputs[1].write("\n\n")
    #Get averages of data and differences
    a_avg, b_avg = tifA.getAvg(y), tifB.getAvg(y)
    a_avg = "n/a" if a_avg == tifA.no_value else a_avg
    b_avg = "n/a" if b_avg == tifB.no_value else b_avg
    dif_avg = "n/a" if len(differences) == 0 else sum(differences)/len(differences)
    #Write analysis into file
    outputs[0].write(
      "total data points: {}\nneither: {}\nonly A: {}\nonly B: {}\nboth: {}\n".format(
      sum(comparison), comparison[0], comparison[1], comparison[2], comparison[3])
      + "average A: {}\naverage B: {}\naverage difference (A - B): {}\n\n".format(
      a_avg, b_avg, dif_avg))
  print ("Results have been written to the compare_{}.out files\n"
         .format(compare_no))

def writeCompareImg(tif1, tif2, path, compare_no):
  print ("Creating comparison heatmap")
  #Get color data
  color_data = tif1.getComparisonImgData(tif2)
  im = Image.fromarray(color_data, 'RGB')
  im.save("{}compare_{}.jpg".format(path, compare_no))
  print ("Heatmap has been saved to compare_{}.jpg\n".format(compare_no))

def isTif(file_name):
  return file_name[file_name.rfind("/") + 1:].endswith(".tif")

if __name__ == '__main__':
  usage = """
    Usage:
    $ python {} -iw input-raster1 -iwc [-iw] input-raster1 [-iw] input-raster2
      [-p path-name]
        input-raster: input tif file
        -i: creates an image from the tif file or comparison
        -w: writes data for tif file or comparison to a .out file
        -c: compares two input files
            use -i and -w normally for each input-raster
    	-p path-name: Output file destination from your current location
    """.format(sys.argv[0])

  #Check arguments
  i = 1
  #Stores custom path
  custom_path = ""
  #Store any relevent arguments
  args = []
  #Store previous arg
  prev_arg = ""
  
  while i < len(sys.argv):
    arg = sys.argv[i]
    if arg == "-p":
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
    #If argument is not a file
    elif '.' not in arg:
      #Check for unfilled space in the last arg
      if len(args) > 0 and '' in args[-1]:
        #If the working arg has -c and current arg has -c
        #Then -c does not have its two input files
        if 'c' in args[-1][0] and 'c' in arg:
          print ("[ Error ] 'c' needs two input files\nSkipping")
          args.pop()
        #If the working arg has -i or -w and current arg is some -
        #The -i/-w does not have its input file
        elif 'c' not in prev_arg and '.' not in prev_arg:
          print ("[ Error ] 'w' and 'i' need an input file\nSkipping")
          args.pop()
      #Check if user is calling a comparison (-c)
      if 'c' in arg:
        args.append([arg,'','','',''])
      #Check if -w or -i are being added to a comparison or not
      elif len(args) == 0 or 'c' not in args[-1][0]:
        args.append([arg,''])
      else:
        args[-1][args[-1].index('')] = arg
    #If argument is a file
    else:
      #Check if it is a tif
      if not isTif(arg):
        print ("[ Error ] file must be of type tif\nSkipping")
      #Check for previous argument
      elif len(args) == 0 or '' not in args[-1]:
        print ("[ Error ] {} requires an argument before it\nSkipping".format(arg))
      #Use input file
      else:
        #Not part of a comparison
        if 'c' not in args[-1][0]:
          args[-1][1] = arg
        else:
          #Find placement index
          idx = args[-1].index('')
          #If index is even, it is the file index and already has an arg before
          if idx % 2 == 0:
            args[-1][idx] = arg
          #Otherwise we need to add an arg
          else:
            args[-1][idx] = "-"
            args[-1][idx + 1] = arg
    prev_arg = arg
    i += 1

  #Check if last argument had enough inputs
  if len(args) > 0 and '' in args[-1]:
    print ("[ Error ] {}\nSkipping".format("'c' needs two input files" if
      'c' in args[-1][0] else "'w' and 'i' need an input file"))
    args.pop()
  
  #Check if we have any arguments
  if len(args) == 0:
    print ("""
    [ Error ] you must supply at least 2 arguments:
    1) -iw input_raster
    OR
    1) -iwc [-iw] input_raster1 [-iw] input_raster2
    {}""".format(usage))
    sys.exit(1)

main(args[0], custom_path);
