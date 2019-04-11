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
      tif.createImage(path)
    #If arg_set not in args, then it is part of a comparison so return tif
    if arg_set not in args:
      return tif
  else:
    #Create tif objects to compare
    tifA = main([arg_set[1], arg_set[2]], path)
    tifB = main([arg_set[3], arg_set[4]], path) 
    print ("\n\33[32mComparing: \33[0m{} and {}".format(
           tifA.file_name, tifB.file_name))
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
             Path(path + "compare_{}.png".format(j)).is_file()):
        j += 1
      #Check if we are writing an output and/or creating an image
      if 'w' in arg_set[0]:
        compareData(tifA, tifB, path, j)
      if 'i' in arg_set[0]:
        tifA.createCompareImage(tifB, path, j)
    else:
      print("Skipping...")  

  #Get rid of this arg_set and move on to the next arg_set
  args.pop(0)
  if (len(args) > 0):
    main(args[0], path)
  else:
    print ("")

def writeData(tif, path):
  #Go through each data point in the band
  #Print data to file
  print ("Writing data to file", end='')
  #Create output file
  output = open(path + tif.file_name[:-4] + ".out", 'w')
  output.write("Max Y = {}\n\n".format(tif.maxY))
  
  #Go through each y value
  for y, vals in enumerate(tif.data):
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
    print ("\rWriting data to file {}%".format(
             round(y*100/(tif.maxY-1))), end="")
  output.close()

  print ("\nData written to {}".format(path + tif.file_name[:-4] + ".out"))



def compareData(tifA, tifB, path, compare_no):
  print ("Writing comparison to file", end='')
  A = 0
  B = 1
  #Get smallest maxY
  maxY = min(tifA.maxY, tifB.maxY)
  #Create file to write to
  #'Simple' has an analysis of the comparison
  #'Raw' has the comparison for every data point
  simple = open(path + "compare_{}_simple.out".format(compare_no), 'w')
  raw = open(path + "compare_{}_raw.out".format(compare_no), 'w')
  #Write the date, time, and input file names on the top of the output files
  for out in [simple, raw]:
    out.write("Created at {}\n".format(datetime.datetime.fromtimestamp(time.time()).
              strftime('%Y-%m-%d %H:%M:%S')))
    out.write("file A is {}, file B is {}\n".format(tifA.file_name, tifB.file_name))
    out.write("Max Y = {}\n".format(maxY))
  simple.write("The simple file contains the totals for each comparison result for each y-value\n\n")
  raw.write("The raw file contains the comparison result for each data point\n\n")

  #Store the analysis text for each y-value in the 'simple' file as a list of strings
  text = []
  #Store all differences and percent differences
  difs, pct_difs = [], []
  five_perc=0
  ten_perc=0
  twentyFive_perc=0
  fifty_perc=0
  seventyFive_perc=0
  oneHundred_perc=0
  #Begin comparison
  #Cycle through the data for each y value
  for y in range(maxY + 1):
    #Write the current y value to each file
    temp = "y = {}: ".format(y)
    #simple.write("y = {}: ".format(y))
    raw.write("y = {}: ".format(y))
    #0 = both no data, 1 = A has data, 2 = B has data, 3 = both have data
    result = [0,0,0,0]
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
        raw.write(", ")
      if a != nvA and b != nvB:
        #Both
        raw.write("[{}|{}]".format(a, b))
        result[3] += 1
        #Record difference and percent difference
        difs.append(float(a) - float(b))
        #TODO: Record percent difference and check if they are above certain threshholds
        pct=abs((a-b)/min.(a,b)) 
        pct_difs.append(pct)

        if pct >= .05:
          five_perc++
          if pct >= .1:
            ten_perc++
            if pct >= .25:
              twentyFive_perc++
              if pct >= .5:
                fifty_perc++
                if pct >= .75:
                  seventyFive_perc++
                  if pct >= 1:
                    oneHundred_perc++


      elif a != nvA and b == nvB:
        #Just A
        raw.write("[{}|NA]".format(a))
        result[1] += 1
      elif a == nvA and b != nvB:
        #Just B
        raw.write("[NA|{}]".format(b))
        result[2] += 1
      else:
        #Neither
        raw.write("NA")
        result[0] += 1
      first = False

    raw.write("\n\n")
    #Record analysis as text
    temp += "total data points: {}\nneither: {}\n".format(sum(result), result[0])
    temp += "only A: {}\nonly B: {}\nboth: {}\n\n".format(result[1], result[2],
                                                          result[3])
    text.append(temp)
    print ("\rWriting comparison to file {}%".format(
             round(y*100/maxY)), end='')
  
  #Write statistics about the data as a whole to the 'simple' file
  #Average values for each file
  simple.wite("A Average: {}\nB Average: {}\n".format(
               tifA.getAvg(-1), tifB.getAvg(-1)))
  #Average difference and percent difference
  avg_dif = "n/a" if len(difs) == 0 else sum(difs)/len(difs)
  avg_pct_dif = "n/a" if len(pct_difs) == 0 else sum(pct_difs)/len(pct_difs)
  simple.write("Average Difference (A - B): {}\n".format(avg_dif))
  simple.write("Average Percent Difference: {}%\n".format(avg_pct_dif*100))
  #TODO: write percent difference counts into file
  simple.write("Percent differences greater than:\n")
  simple.write("5%: {}\n10%: {}\n25%: {}".format(five_perc, ten_perc, twentyFive_perc))
  #do the rest of the percertages
  simple.write("50%: {}\n75%: {}\n100%: {}".format(fifty_perc, seventyFive_perc,oneHundred_perc))
  #Write all the stored text to the 'simple' file
  for txt in text:
    simple.write(txt)
  
  #Close the output files
  simple.close()
  raw.close()

  print ("\nResults have been written to the compare_{}.out files".format(
         compare_no))

def isTif(file_name):
  return file_name[file_name.rfind("/") + 1:].endswith(".tif")

if __name__ == '__main__':
  usage = """
    Usage:
    $ python {} -iw input-tif  OR  -iwc [-iw] input-tif1 [-iw] input-tif2
      [-p path-name]
        input-raster: input tif file
        -i input-tif: creates an image from the tif file or comparison
        -w input-tif: writes data for tif file or comparison to a .out file
        -c input-tif1 input-tif2: compares two input files
            use -i and -w normally for each input-tif
    	-p path-name: Output file destination from your current location
    """.format(sys.argv[0])

  #Stores custom path
  custom_path = ""
  #Store any relevent arguments
  args = []
  #Store previous arg
  prev_arg = ""
  
  i = 1
  while i < len(sys.argv):
    arg = sys.argv[i]
    #If the argument is -p, make sure a path was provided
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
        #If the working arg has -i or -w and current arg is some '-'
        #Then -i/-w does not have its input file
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
        print ("[ Error ] {} requires an argument before it\nSkipping".format(
               arg))
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
