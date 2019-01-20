import sys
import subprocess
import os
import time
import datetime
from pathlib2 import Path

input_files = ["",""]

def main():
  global input_files
  input_files = [sys.argv[1], sys.argv[2]]
  #Check file types
  start_path = os.path.join(os.path.dirname(__file__), "../tif/data/")

  for i in range(2):
    last_slash = input_files[i].rfind("/") + 1
    if input_files[i].endswith(".tif"):
      #If file type is .tif, run gettifinfo.py on the tif file
      print ("\nAnalyzing {}\nResults will be piped to {}.out".format(
             input_files[i][last_slash:], input_files[i][last_slash:-4]))
      subprocess.call(['python', os.path.dirname(__file__) + '/gettifinfo.py', input_files[i]])
      #Change input file to new .out file
      input_files[i] = start_path + "{}.out".format(input_files[i][last_slash:-4])
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
  maxY = [int(contents[A][0][8:]), int(contents[B][0][8:])]
  #Remove every line that doesn't start with 'y'
  contents = [[line for line in contents[A] if line[:1] == "y"],
              [line for line in contents[B] if line[:1] == "y"]]
  A_is_bigger = maxY[A] >= maxY[B]
  
  #Create file to write to
  #'Simple' has the totals for each comparison result
  #'Raw' has the comparison for every data point
  end_path = os.path.join(os.path.dirname(__file__), "../tif/tif_comparisons/")
  #Check to make sure file name doesn't already exist
  j = 1
  while (Path(end_path + "compare_{}_simple.out".format(j)).is_file() or
         Path(end_path + "compare_{}_raw.out".format(j)).is_file()):
    j += 1
  outputs = [open(end_path + "compare_{}_simple.out".format(j), 'w'),
             open(end_path + "compare_{}_raw.out".format(j), 'w')]
  #Write the date, time, and input file names on the top of the output files
  for out in outputs:
    out.write("Created at {}\n".format(datetime.datetime.fromtimestamp(time.time()).
              strftime('%Y-%m-%d %H:%M:%S')))
    out.write("file A is {}, file B is {}\n".format(input_files[A][input_files[A].rfind("/") + 1:],
              input_files[B][input_files[B].rfind("/") + 1:]))
    out.write("Max Y = {}\n".format(maxY[A] if A_is_bigger else maxY[B]))
  outputs[0].write("The simple file contains the totals for each comparison result for each y-value\n\n")
  outputs[1].write("The raw file contains the comparison result for each data point\n\n")

  #Begin comparison
  print ("Beginning comparison\n")
  #Cycle through the data for each y value
  for i in range(maxY[A if A_is_bigger else B] + 1):
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
    #Compare each data point and write the result to the 'raw' file
    while len(line[A]) > 1 and len(line[B]) > 1:
      #Retrieve each data point (separated by ', ')
      data = [line[A][:line[A].find(", ")], line[B][:line[B].find(", ")]]
      if not data[A] == "NA" and not data[B] == "NA":
        outputs[1].write("[A = {}|B = {}], ".format(data[A], data[B]))
        comparison_results[3] += 1
      elif not data[A] == "NA" and data[B] == "NA":
        outputs[1].write("[A = {}|NA], ".format(data[A]))
        comparison_results[1] += 1
      elif data[A] == "NA" and not data[B] == "NA":
        outputs[1].write("[NA|B = {}], ".format(data[B]))
        comparison_results[2] += 1
      elif data[A] == "NA" and data[B] == "NA":
        outputs[1].write("NA, ")
        comparison_results[0] += 1
      #Remove the data point before moving on
      line[A] = line[A][line[A].find(", ") + 2:]
      line[B] = line[B][line[B].find(", ") + 2:]
    
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
  if len( sys.argv ) != 3:
    print ("""
    [ Error ] you must supply 2 arguments:
    1) output file OR tif file  2) output file OR tif file
    
    Usage:
    $ python compareout.py input-file input-file
    """)
    sys.exit(1)

main();
