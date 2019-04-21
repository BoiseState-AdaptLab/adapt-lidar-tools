#File name: tif.py
#Created on: 17-December-2018
#Author: Aaron Orenstein

from __future__ import print_function
from osgeo import gdal
from PIL import Image, ImageDraw, ImageFont
import numpy as np
import math
import sys
import struct
import os
gdal.UseExceptions();

class Tif:
  def __init__(self, file_name):
    self.file_name_full = file_name

    #Get and check file name
    last_slash = file_name.rfind("/") + 1;
    #Cut down string to just filename.tif
    file_name = file_name[last_slash:]
    #Make sure file is of type .tif
    if not file_name.endswith(".tif"):
      print ("Invalid file: {}\nFile types supported: .tif\n".format(
             file_name))
      return
    
    #Initialize
    self.file_name = file_name
    self.file_name_trimmed = file_name[:-4]
    self.getData()
    print ("\n")

  #Gets average values at a y-value, y = -1 gets overall average
  def getAvg(self, y):
    num_data = []
    if y >= 0:
      num_data = [self.data[y, x] for x in range(self.maxX + 1) 
                  if self.data[y, x] != self.no_value]
    else:
      num_data = [z for z in self.data.flatten() if z != self.no_value]
    return self.no_value if len(num_data) == 0 else (sum(num_data) / len(num_data))

  #Get tif data
  def getData(self):
    print ("\33[32mProcessing:\33[0m {}".format(self.file_name))

    #Obtain raster
    raster = gdal.Open(self.file_name_full)
    if raster is None:
      print ('Unable to open %s' % self.file_name_full);
      return

    #Get raster band
    try:
      band = raster.GetRasterBand(1)
    except (RuntimeError, e):
      print ('No band %i found' % 1);
      print (e);
      return

    #Get no data value default -99999
    self.no_value = band.GetNoDataValue()
    if self.no_value is None:
      self.no_value = -99999
  
    #Get the data from the band and return a 2D list
    print ("Getting data", end="")
    
    #Data will be stored here, y = rows, x = columns
    data = np.full((band.YSize, band.XSize), self.no_value, dtype=float)
    #Go through each y value
    for y in range(band.YSize):
      #Read the raster
      scanline = band.ReadRaster(xoff=0, yoff=y,
                                 xsize=band.XSize, ysize=1,
                                 buf_xsize=band.XSize, buf_ysize=1,
                                 buf_type=gdal.GDT_Float32)
      #Get data for all x values
      tuple_of_floats = struct.unpack('f' * band.XSize, scanline)
      #Go through every x value
      for x, val in enumerate(tuple_of_floats):
        #Make sure the data is not the no data value
        if not (val > self.no_value - 1 and val < self.no_value + 1):
          #Add data to data list, inputting as [row, col]
          data[y, x] = val
      print ("\rGetting data {}%".format(
             round(y*100/(band.YSize-1))), end="")

    #Get max y and max x
    self.maxX, self.maxY = band.XSize - 1, band.YSize - 1
    #Save data
    self.data = data

  def createCompareImage(self, tif2, path, compare_no):
    #Dimension conventions
    #Width (w) is always measured in columnns
    #Height (h) is always measured in rows
    #Each column and row has a width/height of 1 pixel (px)

    print ("Creating comparison heatmap", end="")

    #Get tif2 details
    data2 = tif2.data
    #Get width and height
    data_w = min(data2.shape[1], self.data.shape[1])
    data_h = min(data2.shape[0], self.data.shape[0])
    #Create color array
    color_data = np.zeros((data_h, data_w, 3), dtype=np.uint8)
    #Colors- white (lowest), red, black (highest)
    colors = [[255,255,255], [255,0,0], [0,0,0]]

    first_compare = True
    max_dif = 0
    
    #Counters for percent differences above these threshholds
    threshholds = [.05,.1,.25,.5,.75,1]
    counters = [0,0,0,0,0,0]

    #Go through each y value
    for y, (vals1, vals2) in enumerate(zip(self.data, data2)):
      #Go through each x value
      for x, (val1, val2) in enumerate(zip(vals1, vals2)):
        no_value1 = val1 == self.no_value
        no_value2 = val2 == tif2.no_value
        #If neither has data, color is white
        if no_value1 and no_value2:
          color_data[y, x] = [255,255,255]
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
          #Check if that % is our ne biggest
          if first_compare or frac > max_dif:
            max_dif = frac
          #Get difference statistics
          for idx, perc in enumerate(threshholds):
            if frac < perc:
              break
            counters[idx] += 1
          color_data[y, x] = getHeatMapColor(colors, frac)
      print ("\rCreating comparison heatmap {}%".format(
             round(y*100/(data_h-1))), end="")

    #Add space for the legend
    #Height of each line of the legend is the image height / 30
    line_h = math.floor(data_h / 30)
    #Create font
    font_type = os.path.dirname(__file__) + "/../etc/times-new-roman.ttf"
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
    print (count_text)
    #set each line of text
    text = ["0%  >100%",
            "Max Percent Difference: {}%".format(round(max_dif * 100, 2)),
            " " + self.file_name, " " + tif2.file_name,
            "Percent differences greater than:", 
            count_text]
    #Find widths of each line of the legend
    line_w = [font.getsize(t)[0] for t in text]
    #Add space for difference gradient
    line_w[0] *= 2
    #Add space for color key if included
    line_w[2] += line_h
    line_w[3] += line_h
    #Get greatest text width
    legend_w = max(line_w)
    #Height of the legend is 6 lines or 1 line
    #+2.1 for 5 1/2 spaces or +1.2 for 1 space
    legend_h = math.floor(line_h * 8.1)
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
                    fill = getHeatMapColor(colors, j / (line_w[0] / 2)))
        hor_pos += j
        #Max text
        draw.text((hor_pos, vert_pos - vert_offset), " >100%", (0,0,0), font=font)
      #File names with color key
      elif i == 2 or i == 3:
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
    print ("\nHeatmap has been saved to compare_{}.png".format(compare_no))

  def getHeatMapColor(self, colors, val_frac):
    num_colors = len(colors) - 1
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
    return (int(red), int(green), int(blue))
