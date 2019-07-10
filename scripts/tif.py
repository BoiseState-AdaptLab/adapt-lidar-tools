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
                  if math.isfinite(self.data[y, x]) and self.data[y, x]
                  != self.no_value]
    else:
      num_data = [z for z in self.data.flatten() if math.isfinite(z) and
                  z != self.no_value]
    return self.no_data if len(num_data) == 0 else (sum(num_data) / len(num_data))

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
