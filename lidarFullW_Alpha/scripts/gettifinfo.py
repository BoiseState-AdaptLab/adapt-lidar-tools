#File Name: gettifinfo.py
#Author: Aaron Orenstein
#Created On: 20 April 2019

import sys
import os
from tif import Tif

def main(file_name, text_output, image_output, custom_path):
  tif = Tif(file_name)
  if text_output:
    writeData(tif, custom_path)

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

#Stores the tif file name
file_name = ""
#Stores the path to ouput files
custom_path = "./"
#Stores what outputs to create
image_output = False
text_output = False

if __name__ == '__main__':
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
          arg_errors.append("Error: Invalid file type, must have extension '.tif'")
        i += 1
      #No file was input    
      except IndexError:
        arg_errors.append("Error: No input file provided")
    #Custom path
    elif arg == '-p':
      #Make sure next argument is a valid path
      try:
        custom_path = sys.argv[i + 1]
        if not os.path.isdir(custom_path):
          arg_errors.append("Error: Invalid path")
        else:
          custom_path += "/" if custom_path[-1:] != "/" else ""
        i += 1
      except IndexError:
        arg_errors.append("Error: No path provided")
    #Check for any outputs to create
    elif arg in ['-w','-i','-wi','-iw']:
      text_output = 'w' in arg or text_output
      image_output = 'i' in arg or image_output
    else:
      arg_errors.append("Error: Invalid argument")
    #If any errors were added, include the argument
    if l != len(arg_errors):
      arg_errors[l] = "'{}' ".format(sys.argv[i]) + arg_errors[l]
    i += 1

  #Make sure at least '-i' or '-w' were included
  if not image_output and not text_output:
    arg_errors.append("Error: Please include one or both of '-i' and '-w'")

  #Check for argument errors and print them
  if len(arg_errors) != 0:
    for err in arg_errors:
      print (err)
    print(usage)
    sys.exit(1)

main(file_name, text_output, image_output, custom_path)
