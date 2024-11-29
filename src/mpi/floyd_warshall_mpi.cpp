#include "common/graph.hpp"
#include "common/utils.hpp"
#include <mpi.h>
#include <vector>
#include <chrono>
#include <iostream>
#include <cmath>

Graph floydWarshallMPI(const Graph& graph, int rank, int size) {
    const size_t V = graph.size();
    Graph result = graph;
    auto& dist = result.getMatrix();

    // Calculate rows per process
    const int rowsPerProc = V / size;
    const int extraRows = V % size;
    const int myRows = (rank < extraRows) ? rowsPerProc + 1 : rowsPerProc;
    const int startRow = (rank < extraRows) ?
                        rank * (rowsPerProc + 1) :
                        rank * rowsPerProc + extraRows;

    // Buffer for broadcasting k-th row
    std::vector<int> kthRow(V);

    for (size_t k = 0; k < V; k++) {
        // Find the process that owns row k
        int kOwner = 0;
        int tmpRows = 0;
        for (int p = 0; p < size; p++) {
            int pRows = (p < extraRows) ? rowsPerProc + 1 : rowsPerProc;
            if (k >= tmpRows && k < tmpRows + pRows) {
                kOwner = p;
                break;
            }
            tmpRows += pRows;
        }

        // Owner of k-th row prepares data for broadcast
        if (rank == kOwner) {
            kthRow = dist[k];
        }

        // Broadcast k-th row to all processes
        MPI_Bcast(kthRow.data(), V, MPI_INT, kOwner, MPI_COMM_WORLD);

        // Each process updates its portion of rows
        for (size_t i = startRow; i < startRow + myRows; i++) {
            for (size_t j = 0; j < V; j++) {
                if (dist[i][k] != Graph::INF &&
                    kthRow[j] != Graph::INF) {
                    int newDist = dist[i][k] + kthRow[j];
                    if (newDist < dist[i][j]) {
                        dist[i][j] = newDist;
                    }
                }
            }
        }
    }

    // Gather results
    for (size_t i = 0; i < V; i++) {
        int owner = 0;
        int tmpRows = 0;
        for (int p = 0; p < size; p++) {
            int pRows = (p < extraRows) ? rowsPerProc + 1 : rowsPerProc;
            if (i >= tmpRows && i < tmpRows + pRows) {
                owner = p;
                break;
            }
            tmpRows += pRows;
        }
        MPI_Bcast(dist[i].data(), V, MPI_INT, owner, MPI_COMM_WORLD);
    }

    return result;
}

#ifndef TESTING
int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    try {
        // Process command line arguments
        size_t V = (argc > 1) ? std::stoul(argv[1]) : 5;

        // Generate random graph on root process
        Graph graph(V);
        if (rank == 0) {
            graph = Graph::generateRandom(V, 0.3);
            std::cout << "Original graph:" << std::endl;
            graph.print();
        }

        // Broadcast the graph from root to all processes
        for (size_t i = 0; i < V; i++) {
            MPI_Bcast(graph.getMatrix()[i].data(), V, MPI_INT, 0, MPI_COMM_WORLD);
        }

        // Start timing
        auto start = std::chrono::high_resolution_clock::now();

        // Run Floyd-Warshall
        Graph result = floydWarshallMPI(graph, rank, size);

        // End timing
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        // Print results from root process
        if (rank == 0) {
            std::cout << "\nShortest paths:" << std::endl;
            result.print();
            std::cout << "\nExecution time: " << duration.count() << " ms" << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Process " << rank << " Error: " << e.what() << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Finalize();
    return 0;
}
#endif