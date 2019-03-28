#File name: tif.py
#Created on: 17-December-2018
#Author: Aaron Orenstein

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
    
    #Get max y and max x
    self.maxX, self.maxY = band.XSize - 1, band.YSize - 1
    #Save data
    self.data = data
 
  #Create image color data
  def createImage(self, path):
    #Dimension conventions
    #Width (w) is always measured in columnns
    #Height (h) is always measured in rows
    #Each column and row has a width/height of 1 pixel (px)

    print ("Creating Image\n")

    #Get dimensions of data
    data_w, data_h = self.data.shape[1], self.data.shape[0]
    #Min/max values of the data array
    data_values = [i for i in self.data.flatten() if i != self.no_value]
    max_val, min_val = max(data_values), min(data_values)
    #Get the string of these values rounded to 2 decimal places
    max_str, min_str = str(round(max_val, 2)), str(round(min_val, 2))

    #Calculate width of the key
    #1/2 goes to min/max text and spacing, 1/2 goes to sample gradient
    key_w = math.floor(max(100, data_h / 4, data_w / 2))
    #Create biggest string possible for min/max, 2 spaces for spacing
    test_text = max_str + min_str + "  "
    #Scale font size to that test_text and key_w / 2
    font_type = 'times-new-roman.ttf'
    font_size = 1
    font = ImageFont.truetype(font_type, font_size)
    while font.getsize(test_text)[0] < key_w / 2:
      font_size += 1
      font = ImageFont.truetype(font_type, font_size)
    font = ImageFont.truetype(font_type, font_size - 1)
    #Find height of the key in rows
    key_h = font.getsize(test_text)[1]
    #Calculate dimensions of image
    #Width encompasses both the data and the key
    #Height includes the data, the key, and vertical spacing
    img_w, img_h = max(data_w, key_w), data_h + math.floor(key_h * 7 / 5)

    #Create an array that store RGB values for each data point
    color_data = np.full((img_h, img_w, 3), 255, dtype=np.uint8)
    #These are the colors for each coloring 'tier'
    #(low) dark green, yellow, red (high)
    colors = [[0,71,0],[255,255,0],[255,0,0]]

    #Calculate image displacement (If key is bigger than image)
    dx = 0 if data_w >= key_w else math.floor((key_w - data_w) / 2)
    #Loop through each y value for each x value
    for y, vals in enumerate(self.data):
      for x, val in enumerate(vals):
        #Check if value is no data
        if val != self.no_value:
          #Normalize value between 0 and 1
          val_frac = (val - min_val) / (max_val - min_val)
          #write color value to array, inputted as [row, col]
          color_data[y, x + dx] = getHeatMapColor(colors, val_frac)

    #Write data to image
    img = Image.fromarray(color_data, 'RGB')
    draw = ImageDraw.Draw(img)
    #Calculate vertical spacing and sample gradient length
    vert_space = math.floor(key_h / 5)
    grad_len = math.floor(key_w / 2)
    #Get half the verticle offset of text to center text wth gradient
    vert_offset = math.floor(font.getoffset(test_text)[1] / 2)
    #Write min value into image
    x_offset = 0 if data_w <= key_w else math.floor((data_w - key_w) / 2)
    draw.text((x_offset, data_h + vert_space - vert_offset),
              min_str + ' ', (0,0,0), font=font)
    #Draw color gradient into image
    x_offset += font.getsize(min_str + ' ')[0]
    for i in range(grad_len):
      #Get current color
      color = getHeatMapColor(colors, i / grad_len)
      #Draw a vertical line, width = 1px (1 col), height = key height
      draw.line((x_offset + i, data_h + vert_space, x_offset + i,
                img_h - vert_space), fill=color)
    #Write max value into image
    x_offset += i
    draw.text((x_offset, data_h + vert_space - vert_offset),
              ' ' + max_str, (0,0,0), font=font)
    #Save image
    img.save(path + self.file_name[:-4] + '.jpg')

  def getComparisonImgData(self, tif2):
    #Dimension conventions
    #Width (w) is always measured in columnns
    #Height (h) is always measured in rows
    #Each column and row has a width/height of 1 pixel (px)

    print ("Creating Image\n")

    #Get tif2 details
    data2 = tif2.data
    #Get width and height
    data_w = min(data2.shape[1], self.data.shape[1])
    data_h = min(data2.shape[0], self.data.shape[0])
    #Get width of key text, >= 100
    key_w = math.floor(max(100, data_h / 4, data_w / 2))
    #Get max difference and create a string of it rounded to 2 decimals
    tmp = max([abs(a - b) for a, b in zip(self.data.flatten(), data2.flatten())
                   if a != self.no_value and b != tif2.no_value])
    max_dif = str(round(tmp, 2))
    #Get largest string in the key between the max/min difference text
    #And the file A/file B text, with spaces
    test_text = max(["0  " + max_dif, "File AFile B"], key=len)
    #Scale font size to that test_text and key_w / 2
    font_type = 'times-new-roman.ttf'
    font_size = 1
    font = ImageFont.truetype(font_type, font_size)
    while font.getsize(test_text)[0] < key_w / 2:
      font_size += 1
      font = ImageFont.truetype(font_type, font_size)
    font = ImageFont.truetype(font_type, font_size - 1)
    print (font_size)
    #Find height of the key in rows, x2 for both lines of text 
    key_h = font.getsize(test_text)[1] * 2

    #Get image dimensions
    #Width encompanses key and data
    #Height includes the data, the key, and vertical spacing
    img_w = max(key_w, data_w)
    img_h = data_h + math.floor(key_h * 13 / 10)
    #Create color array
    color_data = np.zeros((img_h, img_w, 3), dtype=np.uint8)
    #Colors- white (lowest), red, black (highest)
    colors = [[255,255,255], [255,0,0], [0,0,0]]
    
    #Go through each y value
    for y, (vals1, vals2) in enumerate(zip(self.data, data2)):
      #Go through each x value
      for x, (val1, val2) in enumerate(zip(vals1, vals2)):
        no_value1 = val1 == self.no_value
        no_value2 = val2 == tif2.no_value
        #If neither has data, color is med gray
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
          frac = abs(val1 - val2) / min(val1, val2)
          color_data[y, x] = getHeatMapColor(colors, frac)

    return color_data

def getHeatMapColor(colors, val_frac):
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
