#File Name: analyzetif.py
#Author: Aaron Orenstein
#Created On: 20 April 2019

import sys
import os
import math
import numpy as np
from PIL import Image, ImageDraw, ImageFont
from tif import Tif

def main():
  print ()
  #Stores the tif file name
  file_name = ""
  #Stores the path to ouput files
  custom_path = "./"
  #Stores what outputs to create
  image_output = False
  text_output = False

  #Create the usage message
  usage = """
    Usage:
    $ python {} -f file_name -wi [-p pathname]
       -f filename: Name of tif file to be analyzed
       -w: Creates a .out text file containing the data of the tif file
       -i: Creates a heatmap png of the tif file
       -p pathname: Writes all output files to the given path
          """.format(sys.argv[0])

  #Stores any argument errors
  arg_errors = []

  #Iterate through every argument
  i = 1
  while i < len(sys.argv):
    arg = sys.argv[i]
    #Get length of arg_errors
    l = len(arg_errors)
    #Argument is a file
    if arg == '-f':
      try:
        file_name = sys.argv[i + 1]
        #File does not have '.tif' extension
        if not file_name.endswith(".tif"):
          arg_errors.append("Invalid file type, must have extension '.tif'")
        i += 1
      #No file was input
      except IndexError:
        arg_errors.append("No input file provided")
    #Custom path
    elif arg == '-p':
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

  #Make sure at least '-i' or '-w' were included
  if not image_output and not text_output:
    arg_errors.append("Please include one or both of '-i' and '-w'")

  done = "\n\33[32m[ Complete ]\33[0m:"
  fail = "\33[31m[ Error ]\33[0m:"

  #Check for argument errors and print them
  if len(arg_errors) != 0:
    for err in arg_errors:
      print (fail, err)
    print(usage)
    sys.exit(1)
 
  #Run the tif file
  tif = Tif(file_name)
  if text_output:
    writeData(tif, custom_path)
    print (done, "Data written to {}.out\n".format(
           custom_path + tif.file_name_trimmed))
  if image_output:
    createImage(tif, custom_path)
    print (done, "Heatmap has been saved to {}.png\n".format(
           custom_path + tif.file_name_trimmed))

#Write data to .out file
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
             round(y*100/(tif.maxY))), end="")
  output.close()

#Create heatmap image
def createImage(tif, path):
  #Dimension conventions
  #Width (w) is always measured in columnns
  #Height (h) is always measured in rows
  #Each column and row has a width/height of 1 pixel (px)

  print ("Creating heatmap", end="")

  #Get dimensions of data
  data_w, data_h = tif.data.shape[1], tif.data.shape[0]
  #Min/max values of the data array
  data_values = [i for i in tif.data.flatten() if math.isfinite(i) and
                 i != tif.no_value]
  max_val, min_val = max(data_values), min(data_values)
  #Get the string of these values rounded to 2 decimal places
  max_str, min_str = str(round(max_val, 2)), str(round(min_val, 2))

  data_values.sort()
  extremes = getExtremes(data_values)

  #Create an array that store RGB values for each data point
  color_data = np.full((data_h, data_w, 3), 255, dtype=np.uint8)
  #These are the colors for each coloring 'tier'
  #(low) dark green, yellow, red (high)
  #colors = [[0,71,0],[255,255,0],[255,0,0]]
  colors = [[[128,0,128],[0,0,255]],
           [[0,128,0],[255,255,0]],
           [[255,128,0],[255,0,0]]]

  #Loop through each y value for each x value
  for y, vals in enumerate(tif.data):
    for x, val in enumerate(vals):
      #Check if value is no data
      if val != tif.no_value and math.isfinite(val) and val > 0:
        data_type = 0 if val < extremes[0] else 2 if val > extremes[1] else 1
        min_v = min_val if data_type == 0 else extremes[0] if data_type == 1 else extremes[1]
        max_v = extremes[0] if data_type == 0 else extremes[1] if data_type == 1 else max_val
        print (val, min_v, max_v)
        val_frac = (val - min_val) / (max_val - min_val)
        #write color value to array, inputted as [row, col]
        color_data[y, x] = tif.getHeatMapColor(colors[data_type], val_frac)
    print ("\rCreating heatmap {}%".format(
           round(y*100/(data_h-1))), end="")

  #Add space for the legend
  #Height of each line of the legend is the image height / 30
  line_h = math.floor(data_h / 30)
  #Create font using font file in etc/ folder
  path_to_here = os.path.dirname(__file__)
  path_to_here += "/" if path_to_here != "" else ""
  font_type = path_to_here + "../etc/times-new-roman.ttf"
  font = ImageFont.truetype(font_type, line_h)
  #Width of the legend is the width of the text * 2 for the gradient
  test_text = "  " + min_str + max_str
  legend_w = font.getsize(test_text)[0] * 2
  #Height of the legend is one line + 1.2 for 1 space
  legend_h = math.floor(line_h * 2.2)
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

  #Write data to image
  img = Image.fromarray(color_data, 'RGB')
  draw = ImageDraw.Draw(img)
  #Calculate vertical spacing and sample gradient length
  vert_space = math.floor(line_h / 5)
  grad_len = math.floor(legend_w / 2)
  #Get half the verticle offset of text to center text wth gradient
  vert_offset = math.floor(font.getoffset(test_text)[1] / 2)
  #Write min value into image
  x_offset = 0 if data_w <= legend_w else math.floor((data_w - legend_w) / 2)
  draw.text((x_offset, data_h + line_h - vert_offset),
            min_str + ' ', (0,0,0), font=font)
  #Draw color gradient into image
  x_offset += font.getsize(min_str + ' ')[0]
  for i in range(grad_len + 1):
    frac = i / grad_len
    #Get current color
    val = frac * max_val
    color_set = 0 if val < extremes[0] else 2 if val > extremes[1] else 1
    color = tif.getHeatMapColor(colors[color_set], val_frac)
    #Draw a vertical line, width = 1px (1 col), height = legend height
    draw.line((x_offset + i, data_h + line_h, x_offset + i,
              data_h + (2 * line_h)), fill=color)
  #Write max value into image
  x_offset += i
  draw.text((x_offset, data_h + line_h - vert_offset),
            ' ' + max_str, (0,0,0), font=font)
  #Save image
  img.save(path + tif.file_name[:-4] + '.png')

#@params: data - a sorted list of integers
def getExtremes(data):
  l = len(data)
  #Get the length of half and a quarter of the list
  half = math.floor(l/2)
  quart = math.floor(half/2)
  #Calculate quartile 1 and 3 values
  q1 = (data[quart] if half % 2 == 1 else
    (data[quart-1] + data[quart]) / 2)
  q3 = (data[l-quart] if half % 2 == 1 else
    (data[l-quart] + data[l-1-quart]) / 2)
  #Calculate range of non-outlier data
  extension = 1.5 * (q3 - q1)
  q1 -= extension
  q3 += extension
  return [q1,q3]

#Run it
main()
