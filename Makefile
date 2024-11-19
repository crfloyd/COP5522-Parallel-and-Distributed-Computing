# Makefile for Floyd-Warshall Parallel Implementation

# Default build directory
BUILD_DIR = build

# Default number of processes for MPI tests
MPI_PROCS = 4

.PHONY: all clean test run-serial run-openmp run-mpi

# Build all targets
all: $(BUILD_DIR)
	@echo "Building all targets..."
	@cd $(BUILD_DIR) && cmake .. && make -j

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Clean build files
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

# Run tests
test: all
	@echo "Running tests..."
	@cd $(BUILD_DIR) && ctest --output-on-failure

# Run serial version
run-serial: all
	@echo "Running serial implementation..."
	@$(BUILD_DIR)/floyd_warshall_serial

# Run OpenMP version
run-openmp: all
	@echo "Running OpenMP implementation..."
	@$(BUILD_DIR)/floyd_warshall_openmp

# Run MPI version
run-mpi: all
	@echo "Running MPI implementation..."
	@mpirun -np $(MPI_PROCS) $(BUILD_DIR)/floyd_warshall_mpi

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build all implementations (default)"
	@echo "  clean      - Remove build directory and artifacts"
	@echo "  test       - Run all tests"
	@echo "  run-serial - Run serial implementation"
	@echo "  run-openmp - Run OpenMP implementation"
	@echo "  run-mpi    - Run MPI implementation with $(MPI_PROCS) processes"
	@echo ""
	@echo "Usage examples:"
	@echo "  make                   - Build everything"
	@echo "  make test              - Run all tests"
	@echo "  make run-serial        - Run serial version"
	@echo "  make run-openmp        - Run OpenMP version"
	@echo "  make MPI_PROCS=8 run-mpi  - Run MPI version with 8 processes"