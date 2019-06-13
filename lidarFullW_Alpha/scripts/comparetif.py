#File Name: comparetif.py
#Author: Aaron Orenstein
#Created on: 20 April 2019

import sys
import os
import math
import time
import datetime
import numpy as np
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont
from tif import Tif

def main():
  #Store file names
  file_names = []
  #Store custom path
  custom_path = "./"
  #Store what to do
  text_output = False
  image_output = False

  #Create usage statement 
  usage = """
    Usage:
      $ python {} -f file1 -f file2 -wi [-p pathname]
          -f file1: Name of the file to be compared
          -w: Creates a text file of the data of the comparison
          -i: Creates a heatmap image of the comparison
          -p pathname: Writes all output files to the given path 
          """.format(sys.argv[0])
  
  #Collect argument errors here
  arg_errors = []

  #Loop through every argument
  i = 1
  while i < len(sys.argv):
    #get the number of arg errors
    l = len(arg_errors)
    arg = sys.argv[i]
    #Argument is a file
    if arg == "-f":
      try:
        #Try to get the file from the next arguemnt
        name = sys.argv[i + 1]
        #Make sure we don't already have two files
        if len(file_names) == 2:
          arg_errors.append("Only two files may be compared at a time")
        #Make sure it is a tif file
        elif not name.endswith('.tif'):
          arg_errors.append("Invalid file type, must have the extension '.tif'")
        #Store file name
        else:
          file_names.append(name)
        i += 1
      except IndexError:
        arg_errors.append("No input file provided")
    elif arg == "-p":
      #Make sure next argument is a valid path
      try:
        custom_path = sys.argv[i + 1]
        if not os.path.isdir(custom_path):
          arg_errors.append("Invalid path")
        else:
          custom_path += "/" if custom_path[-1:] != "/" else ""
        i += 1
      except IndexError:
        arg_errors.append("No path provided")
    #Check for any outputs to create
    elif arg in ['-w','-i','-wi','-iw']:
      text_output = 'w' in arg or text_output
      image_output = 'i' in arg or image_output
    else:
      arg_errors.append("Invalid argument")
    #If any errors were added, include the argument
    if l != len(arg_errors):
      arg_errors[l] = "'{}' ".format(sys.argv[i]) + arg_errors[l]
    i += 1

  #Make sure two files were inputed
  if len(file_names) != 2:
    arg_errors.append("Comparison requires two files to be inputted")
  #Make sure at least '-i' or '-w' were included
  if not image_output and not text_output:
    arg_errors.append("Please include one or both of '-i' and '-w'")

  done = "\n\33[32m[ Complete ]\33[0m:"
  warn = "\33[33m[ Warning ]\33[0m:"
  fail = "\33[31m[ Error ]\33[0m:"

  #Check for argument errors and print them
  if len(arg_errors) != 0:
    for err in arg_errors:
      print (fail, err)
    print(usage)
    sys.exit(1)
  
  print ()  

  #Find unused comparison number
  j = 1
  while (Path(custom_path + "compare_{}_simple.out".format(j)).is_file() or
         Path(custom_path + "compare_{}_raw.out".format(j)).is_file() or
         Path(custom_path + "compare_{}.png".format(j)).is_file()):
    j += 1 
 
  #Compare tifs
  tifA = Tif(file_names[0])
  tifB = Tif(file_names[1])
  #Check data length
  if tifA.data.shape != tifB.data.shape:
    print (warn, "Tif files are of unequal length\nTif A is {}, Tif B is {}\n".format(
           tifA.data.shape, tifB.data.shape))
  if text_output:
    compareText(tifA, tifB, custom_path, j)
    print (done, "Results have been written to the compare_{}.out files\n".format(j))
  if image_output:
    compareImage(tifA, tifB, custom_path, j)
    print (done, "Heatmap has been saved to compare_{}.png\n".format(j))

def compareText(tifA, tifB, path, compare_no):
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
  five=0
  ten=0
  twentyFive=0
  fifty=0
  seventyFive=0
  oneHundred=0
  fifty_perc=0
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
        #Record percent difference and check if they are above certain threshholds
        pct=abs((a-b)/min(a,b))
        pct_difs.append(pct)
        if pct >= .05:
          five += 1
          if pct >= .1:
            ten += 1
            if pct >= .25:
              twentyFive += 1
              if pct >= .5:
                fifty_perc += 1
                if pct >= .75:
                  seventyFive += 1
                  if pct >= 1:
                    oneHundred += 1
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
  simple.write("A Average: {}\nB Average: {}\n".format(
               tifA.getAvg(-1), tifB.getAvg(-1)))
  #Average difference and percent difference
  avg_dif = "n/a" if len(difs) == 0 else sum(difs)/len(difs)
  avg_pct_dif = "n/a" if len(pct_difs) == 0 else sum(pct_difs)/len(pct_difs)
  simple.write("Average Difference (A - B): {}\n".format(avg_dif))
  simple.write("Average Percent Difference: {}%\n".format(avg_pct_dif*100))
  #Write percent difference counts into file
  simple.write("\nPercent differences greater than:\n")
  simple.write("5%: {}\n10%: {}\n25%: {}\n".format(five, ten, twentyFive))
  #do the rest of the percertages
  simple.write("50%: {}\n75%: {}\n100%: {}\n\n".format(fifty, seventyFive, oneHundred))
  #Write all the stored text to the 'simple' file
  for txt in text:
    simple.write(txt)

  #Close the output files
  simple.close()
  raw.close()

def compareImage(tif1, tif2, path, compare_no):
  #Dimension conventions
  #Width (w) is always measured in columnns
  #Height (h) is always measured in rows
  #Each column and row has a width/height of 1 pixel (px)

  print ("Creating comparison heatmap", end="")

  #Get data
  data1 = tif1.data
  data2 = tif2.data
  #Get width and height
  data_w = min(data2.shape[1], data1.shape[1])
  data_h = min(data2.shape[0], data1.shape[0])
  #Create color array
  color_data = np.zeros((data_h, data_w, 3), dtype=np.uint8)
  #Colors- white (lowest), red, black (highest)
  colors = [[255,255,255], [255,0,0], [0,0,0]]

  max_dif = abs(data1[0][0] - data2[0][0]) / min(data1[0][0],data2[0][0])

  #Counters for percent differences above these threshholds
  threshholds = [.05,.1,.25,.5,.75,1]
  counters = [0,0,0,0,0,0]

  #Go through each y value
  for y, (vals1, vals2) in enumerate(zip(data1, data2)):
    #Go through each x value
    for x, (val1, val2) in enumerate(zip(vals1, vals2)):
      no_value1 = val1 == tif1.no_value
      no_value2 = val2 == tif2.no_value
      #If neither has data, color is white
      if no_value1 and no_value2:
        color_data[y, x] = [128,128,128]
      #If only A has data, color is blue
      elif not no_value1 and no_value2:
        color_data[y, x] = [0,0,255]
      #If only B has data, color is green
      elif no_value1 and not no_value2:
        color_data[y, x] = [0,255,0]
      #Else analyze the data
      else:
        #Get difference as a % of min
        frac = abs(val1 - val2) / min(val1, val2)
        #Check if that % is our new biggest
        if frac > max_dif:
          max_dif = frac
        #Get difference statistics
        for idx, perc in enumerate(threshholds):
          if frac < perc:
            break
          counters[idx] += 1
        color_data[y, x] = tif1.getHeatMapColor(colors, frac)
    print ("\rCreating comparison heatmap {}%".format(
           round(y*100/(data_h-1))), end="")

  #Add space for the legend
  #Height of each line of the legend is the image height / 30
  line_h = math.floor(data_h / 30)
  #Create font
  path_to_here = os.path.dirname(__file__)
  path_to_here += "/" if path_to_here != "" else ""
  font_type = path_to_here + "../etc/times-new-roman.ttf"
  font = ImageFont.truetype(font_type, line_h)
  #Get relevant threshholds
  start, end = len(counters) - 2, len(counters) - 1
  for idx, count in enumerate(counters):
    if count == 0:
      start = max(0, idx - 2)
      end = idx
      break
  count_text = ""
  for idx, count in enumerate(counters[start:end + 1]):
    count_text += " " if idx != 0 else ""
    count_text += "{}%: {}".format(round(100*threshholds[start+idx]),count)
  #set each line of text
  text = ["0%  >100%",
          "Max Relative Difference: {}%".format(round(max_dif * 100, 2)),
          #" " + tif1.file_name, " " + tif2.file_name,
          "Relative differences greater than:",
          count_text]
  #Find widths of each line of the legend
  line_w = [font.getsize(t)[0] for t in text]
  #Add space for difference gradient
  line_w[0] *= 2
  #Add space for color key if included
  #line_w[2] += line_h
  #line_w[3] += line_h
  #Get greatest text width
  legend_w = max(line_w)
  #Height of the legend is 6 lines or 1 line
  #+2.1 for 5 1/2 spaces or +1.2 for 1 space
  legend_h = math.floor(line_h * 5.8)
  #Append rows for the legend
  extra_rows = np.full((legend_h, data_w, 3), 255, dtype=np.uint8)
  color_data = np.vstack((color_data, extra_rows))
  #Append columns for the legend
  if data_w < legend_w:
    extra_columns = np.full((data_h + legend_h,
                            math.floor((legend_w - data_w) / 2), 3),
                            255, dtype=np.uint8)
    #Add to start and end
    color_data = np.hstack((extra_columns, color_data, extra_columns))

  #Write in text for legend
  img = Image.fromarray(color_data, 'RGB')
  draw = ImageDraw.Draw(img)
  #Get vertical spacing size
  vert_space = math.floor(line_h / 5)
  #Set vertical position
  vert_pos = data_h + line_h
  for i, item in enumerate(text):
    #Get vertical offset of text
    vert_offset = math.floor(font.getoffset(item)[1] / 2)
    #Set horizontal position
    hor_pos = math.floor((max(data_w, legend_w) - line_w[i]) / 2)
    #Difference gradient
    if i == 0:
      #Min text
      draw.text((hor_pos, vert_pos - vert_offset), "0% ", (0,0,0), font=font)
      hor_pos += font.getsize("0% ")[0]
      #Gradient
      for j in range(math.floor(line_w[0] / 2)):
        draw.line((hor_pos + j, vert_pos, hor_pos + j, vert_pos + line_h),
                  fill = tif1.getHeatMapColor(colors, j / (line_w[0] / 2)))
      hor_pos += j
      #Max text
      draw.text((hor_pos, vert_pos - vert_offset), " >100%", (0,0,0), font=font)
    #File names with color key
    elif i == 20 or i == 30:
      #Make both names start where the longest starts
      long_name = max(line_w[2], line_w[3])
      hor_pos = math.floor((max(data_w, legend_w) - long_name) / 2)
      #File color key
      draw.rectangle((hor_pos, vert_pos, hor_pos + line_h, vert_pos + line_h),
                fill=((0,0,255) if i == 2 else (0,255,0)), outline=(0,0,0))
      hor_pos += line_h
      #File name
      draw.text((hor_pos, vert_pos - vert_offset), item, (0,0,0), font=font)
      #No space between file A name and file B name
      if i == 2:
        vert_pos -= math.floor(vert_space / 2)
    #Just text
    else:
      #Write the text
      draw.text((hor_pos, vert_pos - vert_offset), item, (0,0,0), font=font)
    vert_pos += line_h + vert_space
 
    img.save("{}compare_{}.png".format(path, compare_no)) 

main()
