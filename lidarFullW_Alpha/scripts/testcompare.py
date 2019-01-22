#File name: testcompare.py
#Created on: 20-January-2019
#Author: Aaron Orenstein

import sys
import subprocess
import os
from pathlib2 import Path

loc = os.path.dirname(__file__)

def main():
  global loc
  #Create test files
  createFiles()
  #Compare the test files
  subprocess.check_output(['python', loc + 'compareout.py', loc + 'test1.out',
                   loc + 'test2.out', '-p', loc if loc != "" else "./"])
  result = open(loc + "compare_1_raw.out", 'r')
  content = result.readlines()
  print ("Beginning Tests")
  #Check correct max y
  Compare(content[2], "Max Y = 5\n", "Max Y Test")
  #Remove header and empty line
  content = [x for x in content if x[:1] == "y"]
  #Check basic comparisons
  Compare(content[0], "y = 0: NA, [100|NA], [NA|100], [100|100]\n",
          "Comparison Logic Test")
  #Check unordered y's
  Compare(content[1] + content[2], "y = 1: [NA|100], [NA|100], [NA|100], " +
          "[NA|100]\ny = 2: [100|NA], [100|NA], [100|NA], [100|NA]\n",
	  "Unordered Y-values Test")
  #Check missing y's
  Compare(content[3] + content[4] + content[5], "y = 3: File A had no data\n" + 
          "y = 4: File B had no data\ny = 5: File A and File B had no data\n",
	  "Missing Y Test")
  
  #Remove all created files
  print ("\nRemoving created files")
  files = ["test1.out", "test2.out", "compare_1_raw.out", "compare_1_simple.out"]
  for _file in files:
    os.remove(loc + _file)
  
def createFiles():
  global loc
  #Create test file A
  print ("Creating test file A")
  A = open(loc + "test1.out", 'w')
  A.write("Max Y = 5\n\ny = 0: NA, 100, NA, 100\n\n" +
          "y = 1: NA, NA, NA, NA, \n\ny = 2: 100, 100, 100, 100\n\n" +
     	  "y = 4: NA, NA, 100, 100")
  A.close()

  #Create test file B
  print ("Creating test file B\n")
  B = open(loc + "test2.out", 'w')
  B.write("Max Y = 5\n\ny = 0: NA, NA, 100, 100\n\n" +
          "y = 2: NA, NA, NA, NA\n\ny = 1: 100, 100, 100, 100\n\n" +
	  "\n\ny = 3: NA, NA, 100, 100")
  B.close()

def Compare(actual, expected, test_name):
  equal = expected == actual
  print ("\n{} \n{}".format(test_name, "\33[32m[ Passed ]\33[0m" if equal else "\33[31m[ Failed ]\33[0m"))
  if not equal:
    actual_diff = ""
    expected_diff = ""
    for char1, char2 in zip(actual, expected):
      #Highlight differences in red
      if char1 != char2 and actual_diff.find("\33[31m") <= actual_diff.find("\33[0m"):
        actual_diff += "\33[31m"
	expected_diff += "\33[31m"
      elif actual_diff.find("\33[31m") > actual_diff.find("\33[0m"):
        actual_diff += "\33[0m"
	expected_diff += "\33[0m"
      actual_diff += char1
      expected_diff += char2
    print ("\nExpected: {}\nActual: {}".format(expected_diff, actual_diff))

main()
