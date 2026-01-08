#!/bin/sh

#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --exclusive
#SBATCH --time=00:30:00   # Increased time for N=1,000,000 trials
#SBATCH --partition=day

module load gcc/11.2.0
module load cmake/3.22.0

# 1. Compile with -g (needed for perf to map instructions to code)

rm -r build/* test_results/*
mkdir -p test_results/baseline
cd build 
cmake -DCMAKE_CXX_FLAGS="-O3 -g" ..
make
cd ..

N_VALUES="1000 5000 10000"
#N_VALUES="1000"
TRIALS=10

for N in $N_VALUES; do
    echo "--- Benchmarking N=$N ---"
    
    # Trial 1: Capture hardware metrics with perf
    # We save this to a separate file for your report
    echo "Running Perf on Trial 1..."
    srun perf stat -e cycles,instructions,cache-references,cache-misses,L1-dcache-load-misses \
        -o test_results/baseline/perf_N$N.txt ./build/simulate $N 3600 36000 > simulation_results/baseline/result_N$N.txt

    # Trials 2-10: Capture clean wall-clock time
    echo "Running remaining trials for timing..."
    for i in $(seq 2 $TRIALS); do
       { time ./build/simulate $N 3600 36000 ; } 2>> test_results/baseline/time_results_$N.txt > /dev/null
    done
done

echo "Finished All Trials"
