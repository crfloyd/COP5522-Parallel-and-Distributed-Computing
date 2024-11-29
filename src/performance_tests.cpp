#include "common/graph.hpp"
#include <chrono>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <omp.h>
#include <mpi.h>

// Forward declarations of our implementations
Graph floydWarshall(const Graph& graph);
Graph floydWarshallOpenMP(const Graph& graph, int numThreads);
Graph floydWarshallMPI(const Graph& graph, int rank, int size);

struct TestResult {
    std::string implementation;
    size_t vertices;
    int threads_processes;
    double density;
    double time_ms;
    bool success;
};

class PerformanceTester {
public:
    static std::vector<TestResult> runTests(int rank, int size) {
        std::vector<TestResult> results;
        std::vector<size_t> verticesSizes = {100, 200, 500, 1000};
        std::vector<double> densities = {0.1, 0.3, 0.5};
        std::vector<int> threadCounts = {1, 2, 4, 8};

        // Only root process generates test cases
        if (rank == 0) {
            // Serial tests
            for (size_t V : verticesSizes) {
                for (double density : densities) {
                    results.push_back(testSerial(V, density));
                }
            }

            // OpenMP tests
            for (size_t V : verticesSizes) {
                for (double density : densities) {
                    for (int threads : threadCounts) {
                        results.push_back(testOpenMP(V, density, threads));
                    }
                }
            }
        }

        // MPI tests (all processes participate)
        for (size_t V : verticesSizes) {
            for (double density : densities) {
                results.push_back(testMPI(V, density, rank, size));
            }
        }

        return results;
    }

    static void saveResults(const std::vector<TestResult>& results, const std::string& filename) {
        std::ofstream outFile(filename);
        outFile << "Implementation,Vertices,Threads/Processes,Density,Time(ms),Success\n";

        for (const auto& result : results) {
            outFile << result.implementation << ","
                   << result.vertices << ","
                   << result.threads_processes << ","
                   << std::fixed << std::setprecision(2) << result.density << ","
                   << std::fixed << std::setprecision(2) << result.time_ms << ","
                   << (result.success ? "true" : "false") << "\n";
        }
    }

private:
    static TestResult testSerial(size_t V, double density) {
        TestResult result = {"Serial", V, 1, density, 0.0, true};

        try {
            Graph g = Graph::generateRandom(V, density);
            auto start = std::chrono::high_resolution_clock::now();
            Graph res = floydWarshall(g);
            auto end = std::chrono::high_resolution_clock::now();
            result.time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        } catch (...) {
            result.success = false;
        }

        return result;
    }

    static TestResult testOpenMP(size_t V, double density, int threads) {
        TestResult result = {"OpenMP", V, threads, density, 0.0, true};

        try {
            Graph g = Graph::generateRandom(V, density);
            auto start = std::chrono::high_resolution_clock::now();
            Graph res = floydWarshallOpenMP(g, threads);
            auto end = std::chrono::high_resolution_clock::now();
            result.time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        } catch (...) {
            result.success = false;
        }

        return result;
    }

    static TestResult testMPI(size_t V, double density, int rank, int size) {
        TestResult result = {"MPI", V, size, density, 0.0, true};

        try {
            Graph g(V);
            if (rank == 0) {
                g = Graph::generateRandom(V, density);
            }

            // Broadcast the graph to all processes
            for (size_t i = 0; i < V; i++) {
                MPI_Bcast(g.getMatrix()[i].data(), V, MPI_INT, 0, MPI_COMM_WORLD);
            }

            auto start = std::chrono::high_resolution_clock::now();
            Graph res = floydWarshallMPI(g, rank, size);
            auto end = std::chrono::high_resolution_clock::now();

            // Only use timing from root process
            if (rank == 0) {
                result.time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            }
        } catch (...) {
            result.success = false;
        }

        return result;
    }
};

int main(int argc, char* argv[]) {
    int rank = 0, size = 1;

    // Initialize MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    try {
        if (rank == 0) {
            std::cout << "Starting performance tests..." << std::endl;
        }

        auto results = PerformanceTester::runTests(rank, size);

        // Only root process saves results
        if (rank == 0) {
            std::string filename = "performance_results.csv";
            PerformanceTester::saveResults(results, filename);
            std::cout << "Results saved to " << filename << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Process " << rank << " Error: " << e.what() << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Finalize();
    return 0;
}