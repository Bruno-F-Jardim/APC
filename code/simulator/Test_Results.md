# N-Body Simulation: Performance Engineering Log

**Author:** [Your Name/Group]
**Target Architectures:** SeARCH (x86/Intel/GPU) & Deucalion (ARM A64FX)

## 1. System Environment

*Items captured from `lscpu` and `module list` on the cluster.*

| Metric            | SeARCH (CPU)    | Deucalion (ARM)      | GPU (SeARCH)            |
| :---------------- | :-------------- | :------------------- | :---------------------- |
| **Processor**     | Intel           | ARM A64FX            | NVIDIA [Model]          |
| **Cores/Threads** | [e.g., 32C/64T] | [e.g., 48C]          | [e.g., 5120 CUDA Cores] |
| **Vector Ext.**   | AVX-512 / AVX2  | SVE (512-bit)        | N/A                     |
| **Compiler**      | GCC 11.2.0      | [e.g., Arm Compiler] | NVCC                    |

---

## 2. Optimization Phase Results

### Phase 0: Sequential Baseline (Brute Force)

Compiled using -O3

*Ref: Brute force $O(N^2)$ algorithm.*

#### SeARCH (x86)

| Number of objects (N) | Best(s) | Mean(s) | Median(s) | StdDev | IPC  | L1 Cache Miss % |
| --------------------- | ------- | ------- | --------- | ------ | ---- | --------------- |
| 1000                  | 0.309   | 0.319   | 0.314     | 0.012  | 0.93 | 9.37%           |
| 5000                  | 5.908   | 5.958   | 5.921     | 0.089  | 0.80 | 14.74%          |
| 10000                 | 22.727  | 22.841  | 22.815    | 0.121  | 0.78 | 15.89%          |
| 50000                 | 552.436 | 552.618 | 552.618   | 0.257  | 0.76 | 16.94%          |
| 100000                | N/A     | N/A     | N/A       | N/A    | N/A  | N/A             |

### Phase 1: Sequential Data Layout (AoS to SoA)

*Ref: Improving spatial locality.*

#### SeARCH (x86)
| Number of objects (N) | Best(s) | Mean(s) | Median(s) | StdDev | IPC  | L1 Cache Miss % | Speedup (vs P0) |
| --------------------- | ------- | ------- | --------- | ------ | ---- | --------------- | --------------- |
| 1000                  | 0.303   | 0.314   | 0.306     | 0.015  | 0.85 | 0.66%           | 1.02            |
| 5000                  | 5.743   | 5.772   | 5.777     | 0.015  | 0.70 | 9.49%           | 1.03            |
| 10000                 | 22.181  | 22.232  | 22.242    | 0.036  | 0.67 | 10.81%          | 1.02            |
| 50000                 | N/A     | N/A     | N/A       | N/A    | N/A  | N/A             | N/A             |
| 100000                | N/A     | N/A     | N/A       | N/A    | N/A  | N/A             | N/A             |
### Phase 2: Shared Memory Parallelism (OpenMP)

*Ref: Multithreading in multicore.*

| Threads | Time (SeARCH) | Speedup (S) | Efficiency (E) | Time (Deucalion) |
| :------ | :------------ | :---------- | :------------- | :--------------- |
| 1       |               | 1.0         | 100%           |                  |
| 2       |               |             |                |                  |
| 4       |               |             |                |                  |
| 8       |               |             |                |                  |
| 16      |               |             |                |                  |
| 32      |               |             |                |                  |

### Phase 3: Vectorization (SIMD / SVE)

*Ref: Data-level parallelism.*

| Number of objects (N) | Best(s) | Mean(s) | Median(s) | StdDev | IPC  | L1 Cache Miss % | Speedup (vs P0) |
| --------------------- | ------- | ------- | --------- | ------ | ---- | --------------- | --------------- |
| 1000                  | 0.305   | 0.309   | 0.307     | 0.007  | 0.89 | 0.58%           | 1.01            |
| 5000                  | 5.797   | 5.832   | 5.810     | 0.058  | 0.75 | 6.10%           | 1.02            |
| 10000                 | 22.182  | 22.237  | 22.245    | 0.037  | 0.73 | 6.61%           | 1.02            |
| 50000                 | 539.574 | 540.916 | 540.638   | 1.373  | 0.71 | 8.77%           | 1.02            |
| 100000                | N/A     | N/A     | N/A       | N/A    | N/A  | N/A             | N/A             |


### Phase 4: GPU Acceleration (CUDA)

*Ref: Manycore/Accelerator computing.*

| Implementation        | Time (s) | Speedup (vs Baseline) | Memory Bandwidth |
| :-------------------- | :------- | :-------------------- | :--------------- |
| Global Memory         |          |                       |                  |
| Shared Memory (Tiled) |          |                       |                  |
