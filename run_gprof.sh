make clean
sh install.sh -d
bin/geotiff-driver -f etc/Nayani_clipped_test.pls -a 4,8,17
gprof bin/geotiff-driver gmon.out > analysis.txt
