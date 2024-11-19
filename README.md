# Floyd-Warshall Algorithm Parallelization Project

This project implements and analyzes parallel versions of the Floyd-Warshall algorithm for finding all-pairs shortest paths in a weighted graph.

## Directory Structure

```
floyd-warshall-parallel/
├── src/                   # Source code directory
│   ├── common/            # Shared code and utilities
│   ├── serial/            # Serial implementation (baseline)
│   ├── openmp/            # OpenMP parallel implementation
│   └── mpi/               # MPI distributed implementation
├── tests/                 # Unit tests for project components
├── data/                  # Data directory for input and results
│   ├── input/             # Test graph input files
│   └── results/           # Performance testing results
├── docs/                  # Project documentation
└── scripts/               # Performance testing and analysis scripts
```

## Prerequisites

- C++ compiler with C++17 support
- OpenMP support
- MPI implementation (e.g., OpenMPI)
- CMake (>= 3.15) and/or Make

For macOS, install prerequisites using Homebrew:
```bash
brew install cmake
brew install open-mpi
brew install libomp
```

## Building the Project

You can build using either Make or CMake directly:

### Using Make (Recommended for Quick Start)
```bash
# Build all targets
make

# Clean build files
make clean

# See all available commands
make help
```

### Using CMake Directly
```bash
mkdir build
cd build
cmake ..
make
```

## Running Tests

Using Make:
```bash
make test
```

Using CMake:
```bash
cd build
ctest
```

## Running Implementations

Using Make:
```bash
# Run serial version
make run-serial

# Run OpenMP version
make run-openmp

# Run MPI version (default 4 processes)
make run-mpi

# Run MPI with specific number of processes
make MPI_PROCS=8 run-mpi
```

Using CMake build directory:
```bash
# Run from build directory
./build/floyd_warshall_serial
./build/floyd_warshall_openmp
mpirun -np 4 ./build/floyd_warshall_mpi
```

See individual README files in each implementation directory for specific details

## Implementation Variants

1. Serial Implementation (`src/serial/`)
  - Baseline implementation for performance comparison
  - Single-threaded execution

2. OpenMP Implementation (`src/openmp/`)
  - Shared memory parallelization
  - Uses OpenMP for multi-threading

3. MPI Implementation (`src/mpi/`)
  - Distributed memory parallelization
  - Uses MPI for inter-process communication

See individual README files in each implementation directory for specific details.

## Project Structure Details

- `src/common/`: Contains shared code used across all implementations
  - `graph.hpp/cpp`: Graph data structure and utilities
  - `utils.hpp/cpp`: Common utility functions

- `tests/`: Google Test based unit tests
  - `graph_tests.cpp`: Tests for graph class functionality

- `data/`: Contains input data and results
  - `input/`: Test graph files
  - `results/`: Performance measurement data

- `scripts/`: Helper scripts for testing and analysis
  - `generate_graphs.py`: Generate test graphs
  - `analyze_results.py`: Process and visualize results

## License

MIT