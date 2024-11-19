# Serial Floyd-Warshall Implementation

This directory contains the baseline serial implementation of the Floyd-Warshall algorithm for all-pairs shortest paths.

## Implementation Details

The serial implementation serves as our performance baseline and verifies the correctness of the algorithm. It uses a straightforward triple-nested loop approach with O(V³) time complexity.

## Building and Running

### Using Make (From Project Root)
```bash
# Build all implementations (including serial)
make

# Run serial implementation
make run-serial
```

### Using CMake Directly
```bash
# From project root
mkdir build
cd build
cmake ..
make floyd_warshall_serial

# Run the implementation
./floyd_warshall_serial [num_vertices]
```

## Arguments
- `num_vertices`: (Optional) Size of the random graph to generate. Default is 5.

Example:
```bash
# Using Make
make run-serial

# Using CMake build
./build/floyd_warshall_serial 10  # Runs with a 10x10 graph
```

## Output

The program outputs:
1. The original graph matrix
2. The computed all-pairs shortest paths matrix
3. Execution time in milliseconds