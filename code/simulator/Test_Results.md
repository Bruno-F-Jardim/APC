# N-Body Simulation: Performance Engineering Log

**Author:** [Your Name/Group]
**Target Architectures:** SeARCH (x86/Intel/GPU) & Deucalion (ARM A64FX)

## 1. System Environment

*Items captured from `lscpu` and `module list` on the cluster.*

| Metric | SeARCH (CPU) | Deucalion (ARM) | GPU (SeARCH) |
| :--- | :--- | :--- | :--- |
| **Processor** | Intel | ARM A64FX | NVIDIA [Model] |
| **Cores/Threads** | [e.g., 32C/64T] | [e.g., 48C] | [e.g., 5120 CUDA Cores] |
| **Vector Ext.** | AVX-512 / AVX2 | SVE (512-bit) | N/A |
| **Compiler** | GCC 11.2.0 | [e.g., Arm Compiler] | NVCC |

---

## 2. Optimization Phase Results

### Phase 0: Sequential Baseline (Brute Force)

Compiled using -O3

*Ref: Brute force $O(N^2)$ algorithm.*

#### SeARCH (x86)

| Number of objects (N) | Time (s) | IPC | L1 Cache Miss % | Notes |
| :--- | :--- | :--- | :--- | :--- |
| 1 000 | 0.319 | 0.93 | 23.058 | |
| 5 000 | 6.006 | 0.80 | 2.087 | |
| 10 000 | 22.794 | 0.78 | 1.408 | |
| 50 000 | 552.433 | 0.76 | 0.959 | |
| 100 000 | N/A | N/A | N/A | Surpassed the alotted tolerance time |

### Phase 1: Sequential Data Layout (AoS to SoA)

*Ref: Improving spatial locality.*

| Architecture | Time (s) | Speedup (vs P0) | L1 Cache Miss % |
| :--- | :--- | :--- | :--- |
| SeARCH (x86) | | | |

### Phase 2: Shared Memory Parallelism (OpenMP)

*Ref: Multithreading in multicore.*

| Threads | Time (SeARCH) | Speedup (S) | Efficiency (E) | Time (Deucalion) |
| :--- | :--- | :--- | :--- | :--- |
| 1 | | 1.0 | 100% | |
| 2 | | | | |
| 4 | | | | |
| 8 | | | | |
| 16 | | | | |
| 32 | | | | |

### Phase 3: Vectorization (SIMD / SVE)

*Ref: Data-level parallelism.*

| Architecture | Time (s) | Instruction Set | GFLOPS |
| :--- | :--- | :--- | :--- |
| SeARCH (x86) | | AVX-512 | |

### Phase 4: GPU Acceleration (CUDA)

*Ref: Manycore/Accelerator computing.*

| Implementation | Time (s) | Speedup (vs Baseline) | Memory Bandwidth |
| :--- | :--- | :--- | :--- |
| Global Memory | | | |
| Shared Memory (Tiled) | | | |
