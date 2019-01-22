#File name: compareout.py
#Created on: 22-December-2018
#Author: Aaron Orenstein

import sys
import subprocess
import os
import time
import datetime
from pathlib2 import Path

input_files = ["",""]
custom_path = ""

def main():
  global input_files
  global custom_path

  #Check file types
  for i in range(2):
    last_slash = input_files[i].rfind("/") + 1
    if input_files[i].endswith(".tif"):
      #If file type is .tif, run gettifinfo.py on the tif file
      print ("\nAnalyzing {}\nResults will be piped to {}.out".format(
             input_files[i][last_slash:], input_files[i][last_slash:-4]))
      subprocess.call(['python', os.path.dirname(__file__) + '/gettifinfo.py', 
                       input_files[i], '-p', custom_path if custom_path != "" else "./"])
      #Change input file to new .out file
      input_files[i] = custom_path + "{}.out".format(input_files[i][last_slash:-4])
    elif not input_files[i].endswith(".out"):
      print ("\n[ Warning ] ", input_files[i][last_slash:],
             """is not a .tif or .out file.
             If you encounter an error, consider using the .tif
             file itself or running gettifinfo.py on your tif
             file and using the .out file that is created.\n""")
  Compare()

def Compare():
  global input_files
  A = 0
  B = 1
  #Open input files and retrieve contents
  first = open(input_files[A], 'r')
  second = open(input_files[B], 'r')
  contents = [first.readlines(), second.readlines()]
  first.close()
  second.close()
  #Get max Y's from the first line of the file
  maxY = int(contents[A][0][8:])
  if not maxY == int(contents[B][0][8:]):
    print("Tif data files are not of equal length.\n" +
          "File A has a max y of {}. File B has a max y of {}."
	  .format(maxY[A], maxY[B]))
    return
  #Remove every line that doesn't start with 'y'
  #These do not contain data
  contents = [[line for line in contents[A] if line[:1] == "y"],
              [line for line in contents[B] if line[:1] == "y"]]
  
  #Create file to write to
  #'Simple' has the totals for each comparison result
  #'Raw' has the comparison for every data point
  #Check to make sure file name doesn't already exist
  j = 1
  while (Path(custom_path + "compare_{}_simple.out".format(j)).is_file() or
         Path(custom_path + "compare_{}_raw.out".format(j)).is_file()):
    j += 1
  outputs = [open(custom_path + "compare_{}_simple.out".format(j), 'w'),
             open(custom_path + "compare_{}_raw.out".format(j), 'w')]
  #Write the date, time, and input file names on the top of the output files
  for out in outputs:
    out.write("Created at {}\n".format(datetime.datetime.fromtimestamp(time.time()).
              strftime('%Y-%m-%d %H:%M:%S')))
    out.write("file A is {}, file B is {}\n".format(input_files[A][input_files[A].rfind("/") + 1:],
              input_files[B][input_files[B].rfind("/") + 1:]))
    out.write("Max Y = {}\n".format(maxY))
  outputs[0].write("The simple file contains the totals for each comparison result for each y-value\n\n")
  outputs[1].write("The raw file contains the comparison result for each data point\n\n")

  #Begin comparison
  print ("\33[32mBeginning comparison\33[0m\n")
  #Cycle through the data for each y value
  for i in range(maxY + 1):
    line = ["", ""]
    pos = [0, 0]
    #For each file, make sure we are at the correct y value
    for k in range(2):
      if len(contents[k]) == 0:
        pos[k] = -1
	continue
      #Find the y value of the current line
      beginning = line[k].find(":") + 2
      #While that y value is not the correct value, check the next line
      for pos[k] in range(len(contents[k])):
        #Get y value of new line
	line[k] = contents[k][pos[k]]
	beginning = line[k].find(": ") + 2
        if line[k][:beginning] == "y = {}: ".format(i):
	  break
        elif len(contents[k]) == pos[k] + 1:
	  pos[k] = -1
	  break
    #Write the current y value to the 'raw' file
    outputs[0].write("y = {}: ".format(i))
    outputs[1].write("y = {}: ".format(i))
    #If one or both files had nothing at that y, say so and skip it
    if -1 in pos:
      text = "A" if not pos[B] == -1 else "B" if not pos[A] == -1 else "A and File B"
      outputs[0].write("File {} had no data\n\n".format(text))
      outputs[1].write("File {} had no data\n\n".format(text))
      #Get rid of any unnecessary data
      if not pos[A] == -1:
        contents[A].pop(pos[A])
      elif not pos[B] == -1:
        contents[B].pop(pos[B])
      continue
    #Remove the y value text
    line[A] = line[A][beginning:]
    line[B] = line[B][beginning:]

    #0 = both no data, 1 = A has data, 2 = B has data, 3 = both have data
    comparison_results = [0,0,0,0]
    #Get the data
    data = [[],[]]
    for m in range(0, 2):
      #Get all data points
      while ", " in line[0]:
        cut = line[0].find(", ")
	data[m].append(line[0][:cut])
	line[0] = line[0][cut + 2:]
      #Get last data point
      data[m].append(line[0][:-1])
      line.pop(0)
    first = True
    #Compare each data point and write the result to the 'raw' file
    for a, b in zip(data[A], data[B]):
      #', ' is added before the next data comparison
      #This way is will not be added after the last data point
      #However we don't want it before the first data point
      if not first:
        outputs[1].write(", ")
      if not a == "NA" and not b == "NA":
        outputs[1].write("[{}|{}]".format(a, b))
        comparison_results[3] += 1
      elif not a == "NA" and b == "NA":
        outputs[1].write("[{}|NA]".format(a))
        comparison_results[1] += 1
      elif a == "NA" and not b == "NA":
        outputs[1].write("[NA|{}]".format(b))
        comparison_results[2] += 1
      elif a == "NA" and b == "NA":
        outputs[1].write("NA")
        comparison_results[0] += 1
      first = False
    
    outputs[1].write("\n\n")
    #remove the completed line
    contents[A].pop(pos[A])
    contents[B].pop(pos[B])
    #Write frequency of comparison results to the 'simple' file
    outputs[0].write("total data points: {}\nneither: {}\nonly A: {}\nonly B: {}\nboth: {}\n\n"
                     .format(sum(comparison_results), comparison_results[0], comparison_results[1],
                     comparison_results[2], comparison_results[3]))
  print ("Comparison complete\nResults have been written to the compare_{}.out files\n".format(j))

if __name__ == '__main__':
  usage = """
    Usage:
    $ python {} input-file input-file [-p path_name]
    	-p path_name: Output file destination from your current location
    """.format(sys.argv[0])

  #Check arguments
  j = 1
  while j < len(sys.argv):
    if sys.argv[j] == "-p":
      #Make sure next argument is a valid path
      j += 1
      if len(sys.argv) == j or not os.path.isdir(sys.argv[j]):
        print ("""
    [ Error ] invalid or missing path
	
    {}""".format(usage))
        sys.exit(1)
      else:
        custom_path = sys.argv[j]
    else:
      #Store any input files
      for k in xrange(len(input_files)):
        if input_files[k] == "":
          input_files[k] = sys.argv[j]
          break
    j += 1
  if "" in input_files:  
    print ("""
    [ Error ] you must supply at least 2 arguments:
    1) output file OR tif file  2) output file OR tif file

    {}""".format(usage))
    sys.exit(1)

main();
