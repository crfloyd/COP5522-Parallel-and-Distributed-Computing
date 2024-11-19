# OpenMP Parallel Floyd-Warshall Implementation

This directory contains the shared-memory parallel implementation of the Floyd-Warshall algorithm using OpenMP.

## Implementation Details

The OpenMP implementation parallelizes the algorithm using shared memory multi-threading, focusing on:
- Loop parallelization
- Cache optimization
- Load balancing

## Requirements

- OpenMP-capable compiler
- libomp (on macOS)

For macOS:
```bash
brew install libomp
```

## Building and Running

### Using Make (From Project Root)
```bash
# Build all implementations (including OpenMP)
make

# Run OpenMP implementation
make run-openmp
```

### Using CMake Directly
```bash
# From project root
mkdir build
cd build
cmake ..
make floyd_warshall_openmp

# Run the implementation
./floyd_warshall_openmp [num_vertices] [num_threads]
```

## Arguments
- `num_vertices`: (Optional) Size of the random graph to generate. Default is 5.
- `num_threads`: (Optional) Number of OpenMP threads to use. Default is available cores.

Example:
```bash
# Using Make
make run-openmp

# Using CMake build
./build/floyd_warshall_openmp 10 4  # Runs with a 10x10 graph using 4 threads
```

## Environment Variables

- `OMP_NUM_THREADS`: Set the number of OpenMP threads
- `OMP_SCHEDULE`: Control loop scheduling strategy

Example:
```bash
export OMP_NUM_THREADS=4
export OMP_SCHEDULE="dynamic"
make run-openmp
```