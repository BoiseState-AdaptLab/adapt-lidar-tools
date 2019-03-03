#File name: tif.py
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
    self.getData()

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
    print ("\33[32mProcessing:\33[0m {}\n".format(self.file_name))

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
    print ("Getting Data\n")
    
    #Data will be stored here, y = rows, x = colums
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
    
    #Get max y and max x
    self.maxX, self.maxY = band.XSize - 1, band.YSize - 1
    #Save data
    self.data = data
 
  #Create image color data
  def getImgData(self):
    #Create the image
    print ("Creating Image\n")
    #Width is the number of columns
    #Height is the number of rows
    w, h = self.data.shape[1], self.data.shape[0]
    #Min/max values of the data array
    data_values = [i for i in self.data.flatten() if i != self.no_value]
    max_val, min_val = max(data_values), min(data_values)
    #Create an array that store RGB values for each data point
    #h = number of rows, w = number of collums
    color_data = np.full((h, w, 3), 255, dtype=np.uint8)
    #These are the colors for each coloring 'tier'
    #(low) dark green, yellow, red (high)
    colors = [[0,128,0],[255,255,0],[255,0,0]]
    num_colors = len(colors) - 1

    #Loop through each y value for each x value
    for y, vals in enumerate(self.data):
      for x, val in enumerate(vals):
        #Check if value is no data
        if val != self.no_value:
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
          #write color value to array, inputted as [row, col]
          color_data[y, x] = [red, green, blue]
    return color_data

  def getComparisonImgData(self, tif2):
    #Get tif2 details
    data2 = tif2.data
    #Get width and height
    w = min(data2.shape[1], self.data.shape[1])
    h = min(data2.shape[0], self.data.shape[0])
    #Create color array, h is num of rows, w is num of colums
    color_data = np.zeros((h, w, 3), dtype=np.uint8)
    
    #Go through each y value
    for y, (vals1, vals2) in enumerate(zip(self.data, data2)):
      #Go through each x value
      for x, (val1, val2) in enumerate(zip(vals1, vals2)):
        no_value1 = val1 == self.no_value
        no_value2 = val2 == tif2.no_value
        #If nether has data, color is med gray
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
          #Get difference between the values
          dif = val1 - val2
          #Get dif/min_value from 0 to 1, this is our color fraction
          frac = abs(min(1, dif / min(val1, val2)))
          #Get color val out of frac
          #First 255 goes from white to main color
          #Second 255 goes from main color to black
          support_color = 255 * (1 - min(1, frac * 2))
          main_color = 255 * (1 - max(0, (frac - .5) * 2)) 
          #write color value to array, inputted as [row, col]
          color_data[y, x] = [main_color, support_color, support_color]

    return color_data
