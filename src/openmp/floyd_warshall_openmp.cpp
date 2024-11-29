#include "common/graph.hpp"
#include "common/utils.hpp"
#include <omp.h>
#include <chrono>
#include <iostream>
#include <vector>

Graph floydWarshallOpenMP(const Graph& graph, int numThreads = 0) {
    size_t V = graph.size();
    Graph result = graph;
    auto& dist = result.getMatrix();

    if (numThreads > 0) {
        omp_set_num_threads(numThreads);
    }

    for (size_t k = 0; k < V; k++) {
#pragma omp parallel for schedule(static)
        for (size_t i = 0; i < V; i++) {
            for (size_t j = 0; j < V; j++) {
                if (dist[i][k] != Graph::INF &&
                    dist[k][j] != Graph::INF) {
                    int newDist = dist[i][k] + dist[k][j];
                    if (newDist < dist[i][j]) {
                        dist[i][j] = newDist;
                    }
                    }
            }
        }
    }

    return result;
}

#ifndef TESTING
void runPerformanceTest(size_t V, int numThreads) {
    Graph graph = Graph::generateRandom(V, 0.3);

    auto start = std::chrono::high_resolution_clock::now();
    Graph result = floydWarshallOpenMP(graph, numThreads);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Size: " << V << ", Threads: " << numThreads
              << ", Time: " << duration.count() << "ms" << std::endl;
}

int main(int argc, char* argv[]) {
    try {
        size_t V = (argc > 1) ? std::stoul(argv[1]) : 5;
        int numThreads = (argc > 2) ? std::stoi(argv[2]) : omp_get_max_threads();

        std::cout << "Running Floyd-Warshall with OpenMP" << std::endl;
        std::cout << "Vertices: " << V << std::endl;
        std::cout << "Threads: " << numThreads << std::endl;

        Graph graph = Graph::generateRandom(V, 0.3);

        std::cout << "\nOriginal graph:" << std::endl;
        graph.print();

        auto start = std::chrono::high_resolution_clock::now();
        Graph result = floydWarshallOpenMP(graph, numThreads);
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "\nShortest paths:" << std::endl;
        result.print();
        std::cout << "\nExecution time: " << duration.count() << " ms" << std::endl;

        if (argc > 3) {
            std::cout << "\nRunning performance tests..." << std::endl;
            std::vector<size_t> testSizes = {100, 200, 500, 1000};
            for (auto size : testSizes) {
                runPerformanceTest(size, numThreads);
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
#endif