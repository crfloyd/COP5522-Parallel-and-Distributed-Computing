cmake_minimum_required(VERSION 3.15)
project(floyd_warshall_parallel VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add src directory to include paths
include_directories(${PROJECT_SOURCE_DIR}/src)

# Include FetchContent for downloading GoogleTest
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# macOS specific OpenMP configuration
if(APPLE)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang")
        set(OpenMP_C "${CMAKE_C_COMPILER}")
        set(OpenMP_C_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
        set(OpenMP_C_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")
    endif()
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
        set(OpenMP_CXX_FLAGS "-Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include")
        set(OpenMP_CXX_LIB_NAMES "omp")
        set(OpenMP_omp_LIBRARY "/opt/homebrew/opt/libomp/lib/libomp.dylib")
    endif()
endif()

find_package(OpenMP)
find_package(MPI)

set(COMMON_SOURCES
        src/common/graph.cpp
        src/common/utils.cpp
)

# Create common library
add_library(common_lib ${COMMON_SOURCES})
target_include_directories(common_lib PUBLIC ${PROJECT_SOURCE_DIR}/src)

# Serial implementation
add_executable(floyd_warshall_serial src/serial/floyd_warshall_serial.cpp)
target_link_libraries(floyd_warshall_serial common_lib)

# OpenMP implementation if available
if(OpenMP_CXX_FOUND)
    add_executable(floyd_warshall_openmp src/openmp/floyd_warshall_openmp.cpp)
    target_link_libraries(floyd_warshall_openmp common_lib OpenMP::OpenMP_CXX)
endif()

# MPI implementation if available
if(MPI_CXX_FOUND)
    add_executable(floyd_warshall_mpi src/mpi/floyd_warshall_mpi.cpp)
    target_link_libraries(floyd_warshall_mpi common_lib MPI::MPI_CXX)
endif()

# Enable testing
enable_testing()
add_executable(graph_tests tests/graph_tests.cpp)
target_link_libraries(graph_tests
        common_lib
        GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(graph_tests)