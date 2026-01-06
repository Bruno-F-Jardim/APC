#!/bin/sh
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --exclusive
#SBATCH --time=00:05:00
#SBATCH --partition=day

# Consider using SBATCH --exclusive option outside of the class
# It ensures that no other user pollutes your measurements

module load gcc/11.2.0
module load cmake/3.22.0

echo "Compiling..."
cd build 
cmake -DCMAKE_CXX_FLAGS="-pg" ..
make

echo "Running..."

echo "Running Gprof..."
./simulate 30 3600 36000

# After execution, gprof creates a 'gmon.out' file. 
# We convert it to a readable text file:
gprof ./simulate gmon.out > profile_results.txt
srun perf stat -e cycles,instructions,cache-references,cache-misses,L!-dcache-load-misses ./simulate 30 3600 36000

echo "Finished"