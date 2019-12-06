#!/bin/bash

#Submit this script from root with: sbatch scripts/run_gprof.sh

#SBATCH --time=1:00:00   # walltime
#SBATCH --ntasks=1   # number of processes (i.e. tasks)
#SBATCH --nodes=1   # number of nodes
#SBATCH -p adaptlab   # partition(s)
#SBATCH --mem-per-cpu=1G   # memory per CPU core
#SBATCH -J "lidarProfiling"   # job name
#SBATCH --mail-user=CHANGE_ME   # email address
#SBATCH --mail-type=BEGIN
#SBATCH --mail-type=END

module load gsl/gcc/2.4
module load gdal/intel/2.2.2
make clean
make PROFILER_BUILD=1 geotiff-driver
bin/geotiff-driver -f etc/Nayani_clipped_test.pls -w 13,11,14 -a 4,8,17 -e 1,2,3
gprof bin/geotiff-driver | deps/gprof2dot/gprof2dot.py -s -w | dot -Tpng -o prof.png
mv *.tif gmon.out bin

