import sys
import subprocess
import os
import time
import datetime
from pathlib import Path

input_files = ["",""]

def main():
  global input_files
  input_files = [sys.argv[1], sys.argv[2]]
  #Check file types
  start_path = os.path.join(os.path.dirname(__file__), "../tif/data/")
  for i in range(2):
    last_slash = input_files[i].rfind("/") + 1
    if input_files[i].endswith(".tif"):
      #Run gettifinfo.py on the tif file
      #Change input file to the new .out file
      print ("\nAnalyzing {}\nResults will be piped to {}.out".format(
             input_files[i][last_slash:], input_files[i][last_slash:-4]))
      subprocess.call(['python', os.path.dirname(__file__) + '/gettifinfo.py', input_files[i]])
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
  #Access files
  first = open(input_files[A], 'r')
  second = open(input_files[B], 'r')
  contents = [first.readlines(), second.readlines()]
  first.close()
  second.close()
  #Get max Y's from the first line of the file
  maxY = [int(contents[A][0][9:]), int(contents[B][0][9:])]
  for c in contents:
    c.pop(0)
    c.pop(1)
  A_is_bigger = maxY[A] >= maxY[B]
  #Create file to write to
  #Simple has the totals for each comparison result
  #Raw has the comparison for every data point
  end_path = os.path.join(os.path.dirname(__file__), "../tif/tif_comparisons/")
  j = 1
  while (Path(end_path + "compare_{}_simple.out".format(j)).is_file() or
         Path(end_path + "compare_{}_raw.out".format(j)).is_file()):
    j += 1
  outputs = [open(end_path + "compare_{}_simple.out".format(j), 'w'),
             open(end_path + "compare_{}_raw.out".format(j), 'w')]
  for out in outputs:
    out.write("Created at {}\n".format(datetime.datetime.fromtimestamp(time.time()).
              strftime('%Y-%m-%d %H:%M:%S')))
    out.write("file A is {}, file B is {}\n".format(input_files[A][input_files[A].rfind("/") + 1:],
              input_files[B][input_files[B].rfind("/") + 1:]))
  outputs[0].write("The simple file contains the totals for each comparison result for each y-value\n\n")
  outputs[1].write("The raw file contains the comparison result for each data point\n\n")
  #Begin comparison
  print ("Beginning comparison\n")
  #Cycle through each set of data
  for i in range(maxY[A if A_is_bigger else B]):
    while contents[A][i] == "\n":
      contents[A].pop(i)
    while contents[B][i] == "\n":
      contents[B].pop(i)
    #Get the data
    line = [contents[A][i], contents[B][i]]
    beginning = line[A].find(":") + 2
    outputs[1].write(line[A][:beginning])
    line[A] = line[A][beginning:]
    line[B] = line[B][beginning:]
    #0 = both no data, 1 = A has data, 2 = B has data, 3 = both have data
    comparison_results = [0,0,0,0]
    #Compare each data point
    while len(line[A]) > 1 and len(line[B]) > 1:
      data = [line[A][:line[A].find(", ")], line[B][:line[B].find(", ")]]
      if not data[A] == "NA" and not data[B] == "NA":
        outputs[1].write("[A = {}|B = {}], ".format(data[A], data[B]))
        comparison_results[3] += 1
      elif not data[A] == "NA" and data[B] == "NA":
        outputs[1].write("[NA|B = {}], ".format(data[B]))
        comparison_results[1] += 1
      elif data[A] == "NA" and not data[B] == "NA":
        outputs[1].write("[A = {}|NA], ".format(data[B]))
        comparison_results[2] += 1
      elif data[A] == "NA" and data[B] == "NA":
        outputs[1].write("[NA|NA], ")
        comparison_results[0] += 1
      line[A] = line[A][line[A].find(", ") + 2:]
      line[B] = line[B][line[B].find(", ") + 2:]
    outputs[1].write("\n\n")
    outputs[0].write("y = {}\ntotal data points: {}\nneither: {}\nonly A: {}\nonly B: {}\nboth: {}\n\n"
                     .format(i, sum(comparison_results), comparison_results[0], comparison_results[1],
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
