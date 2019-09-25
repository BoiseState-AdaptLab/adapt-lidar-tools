make clean
make PROFILER_BUILD=1 geotiff-driver
bin/geotiff-driver -f etc/Nayani_clipped_test.pls -a 4,8,17
gprof bin/geotiff-driver gmon.out > analysis.txt
mv *.tif gmon.out analysis.txt bin
