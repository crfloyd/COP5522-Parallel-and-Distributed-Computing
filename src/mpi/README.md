# MPI Parallel Floyd-Warshall Implementation

This directory contains the distributed-memory parallel implementation of the Floyd-Warshall algorithm using MPI.

## Implementation Details

The MPI implementation distributes the computation across multiple processes, with:
- Matrix distribution strategies
- Inter-process communication optimization
- Load balancing considerations

## Requirements

- MPI implementation (e.g., OpenMPI)

For macOS:
```bash
brew install open-mpi
```

## Building and Running

### Using Make (From Project Root)
```bash
# Build all implementations (including MPI)
make

# Run MPI implementation (default 4 processes)
make run-mpi

# Run with specific number of processes
make MPI_PROCS=8 run-mpi
```

### Using CMake Directly
```bash
# From project root
mkdir build
cd build
cmake ..
make floyd_warshall_mpi

# Run the implementation
mpirun -np [num_processes] ./floyd_warshall_mpi [num_vertices]
```

## Arguments
- `num_processes`: Number of MPI processes to launch
- `num_vertices`: (Optional) Size of the random graph to generate. Default is 5.

Example:
```bash
# Using Make
make MPI_PROCS=4 run-mpi

# Using CMake build
mpirun -np 4 ./build/floyd_warshall_mpi 10  # Runs with a 10x10 graph on 4 processes
```

## Performance Considerations

- Process placement affects communication overhead
- Matrix size should be considered when choosing number of processes
- Network topology can significantly impact performance

Example with process binding:
```bash
# Using Make
make MPI_PROCS=4 run-mpi

# Using direct MPI run with process binding
mpirun --map-by core -np 4 ./build/floyd_warshall_mpi 10
```