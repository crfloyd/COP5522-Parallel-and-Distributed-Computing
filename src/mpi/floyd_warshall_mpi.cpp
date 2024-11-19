#include "../common/graph.hpp"
#include "../common/utils.hpp"
#include <mpi.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    // TODO: Implement MPI version
    MPI_Finalize();
    return 0;
}