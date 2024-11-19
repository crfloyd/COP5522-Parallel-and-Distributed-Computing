# Parallel Floyd-Warshall Algorithm Implementation

This project implements and analyzes parallel versions of the Floyd-Warshall algorithm for finding all-pairs shortest paths in a weighted graph.

## Project Structure

- `src/`: Source code for all implementations
  - `common/`: Shared code and utilities
  - `serial/`: Serial implementation (baseline)
  - `openmp/`: OpenMP-based parallel implementation
  - `mpi/`: MPI-based distributed implementation
- `tests/`: Test cases and verification code
- `data/`: Input graphs and performance results
- `scripts/`: Utility scripts for testing and analysis
- `docs/`: Project documentation

## Prerequisites

- CMake (>= 3.15)
- C++ compiler with C++17 support
- OpenMP support
- MPI implementation (e.g., OpenMPI)

For macOS, install prerequisites using Homebrew:
```bash
brew install cmake
brew install open-mpi
```

## Building the Project

```bash
mkdir build
cd build
cmake ..
make
```

## Running Tests

```bash
cd build
ctest
```

## License

[Your chosen license]
