from osgeo import gdal
import sys
import struct
import os
gdal.UseExceptions();

files_complete = 0
file_name = ""

def main(band_num, input_file):
  #Make sure input file is .tif
  if input_file == "-i":
    Finish(0)
  last_slash = input_file.rfind("/") + 1;
  global file_name
  file_name = input_file[last_slash:]
  print ("\n\33[32mProcessing:\33[0m {}\n".format(file_name))
  if not input_file.endswith(".tif"):
    print ("Invalid file\nFile types supported: .tif")
    Finish(1)        

  #Obtain raster
  raster = gdal.Open( input_file )
  if raster is None:
    print ('Unable to open %s' % input_file);
    Finish(1)

  try:
    band = raster.GetRasterBand(band_num)
  except (RuntimeError, e):
    print ('No band %i found' % band_num);
    print (e);
    Finish(1)

  if band.GetNoDataValue() == None:
    band.SetNoDataValue(-99999)
  if "-i" in sys.argv:
    print ("[ NO DATA VALUE ] = ", band.GetNoDataValue());
    print ("[ MIN ] = ", band.GetMinimum());
    print ("[ MAX ] = ", band.GetMaximum());
    print ("[ SCALE ] = ", band.GetScale());
    print ("[ UNIT TYPE ] = ", band.GetUnitType());
  ctable = band.GetColorTable()

  if ctable is None:
    print ('No ColorTable found');
  else:
    print ("[ COLOR TABLE COUNT ] = ", ctable.GetCount());
    for i in range( 0, ctable.GetCount() ):
      entry = ctable.GetColorEntry( i )
      if not entry:
        continue;
      print ("[ COLOR ENTRY RGB ] = ", ctable.GetColorEntryAsRGB( i, entry ));

  Data(band);

def Data(band):
  raster_array = band.ReadAsArray();
  no_value = band.GetNoDataValue();
  #Go through each piece of the band
  #Print data to file
  print ("\nWriting data to file")
  global file_name
  output = open(os.path.join(os.path.dirname(__file__), "../tif/data/") + file_name[:-4] + ".out", 'w')
  output.write("Max Y = {}\n\n".format(band.YSize - 1))
  for i in range(band.YSize):
    output.write("y = {}: ".format(i))
    scanline = band.ReadRaster(xoff=0, yoff=i,
                               xsize=band.XSize, ysize=1,
                               buf_xsize=band.XSize, buf_ysize=1,
                               buf_type=gdal.GDT_Float32)
    tuple_of_floats = struct.unpack('f' * band.XSize, scanline)
    #Print out data
    for val in tuple_of_floats:
      #Check for data
      if not (val > band.GetNoDataValue() - 1 and val < band.GetNoDataValue() + 1):
        output.write("{}, ".format(str(val)))
      else:
        output.write("NA, ")
    output.write("\n\n") 
  output.close()
  Finish(0);

def Usage():
  print("""
  Usage:
  $ python gettifinfo.py input-raster [-i]
      -i: Prints extra information about the tif file
  """)
  sys.exit(1)

def Finish(err):
  global files_complete
  files_complete += 1
  #Check for extra input files
  if len( sys.argv ) >= 2 + files_complete:
    main(1, sys.argv[1 + files_complete])
  else:
    print ("\nNo more files\n")
    sys.exit(err)

if __name__ == '__main__':

  if len( sys.argv ) < 2 or len( sys.argv ) < 3 and "-i" in sys.argv:
    print ("""
    [ ERROR ] you must supply at least one argument:
    1) input raster
    """);
    Usage();

main(1, sys.argv[1]);
