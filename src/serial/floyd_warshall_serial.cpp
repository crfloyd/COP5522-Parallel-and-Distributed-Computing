// src/serial/floyd_warshall_serial.cpp
#include "common/graph.hpp"
#include "common/utils.hpp"
#include <chrono>
#include <iostream>

Graph floydWarshall(const Graph& graph) {
    size_t V = graph.size();
    Graph result = graph; // Create a copy of the input graph
    auto& dist = result.getMatrix();

    // Main Floyd-Warshall algorithm
    for (size_t k = 0; k < V; k++) {
        for (size_t i = 0; i < V; i++) {
            for (size_t j = 0; j < V; j++) {
                if (dist[i][k] != Graph::INF &&
                    dist[k][j] != Graph::INF &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    }
            }
        }
    }

    return result;
}

#ifndef TESTING
int main(int argc, char* argv[]) {
    try {
        // Create a test graph
        size_t V = 5; // Default size for testing
        if (argc > 1) {
            V = std::stoul(argv[1]);
        }

        // Generate a random graph with 30% density
        Graph graph = Graph::generateRandom(V, 0.3);

        std::cout << "Original graph:" << std::endl;
        graph.print();

        // Measure execution time
        auto start = std::chrono::high_resolution_clock::now();

        Graph result = floydWarshall(graph);

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "\nShortest paths:" << std::endl;
        result.print();

        std::cout << "\nExecution time: " << duration.count() << " ms" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
#endif