#pragma once
#include <vector>
#include <limits>
#include <stdexcept>
#include <iostream>

class Graph {
public:
    // Constructor for creating a graph with n vertices
    explicit Graph(size_t vertices);

    // Add weighted edge from source to destination
    void setEdge(size_t src, size_t dest, int weight);

    // Get weight of edge from source to destination
    int getEdge(size_t src, size_t dest) const;

    // Get number of vertices
    size_t size() const { return numVertices; }

    // Print the distance matrix
    void print() const;

    // Create a random graph with given number of vertices and density
    static Graph generateRandom(size_t vertices, double density = 0.3,
                              int minWeight = 1, int maxWeight = 100);

    // Get the underlying matrix for direct manipulation in algorithms
    std::vector<std::vector<int>>& getMatrix() { return adjacencyMatrix; }
    const std::vector<std::vector<int>>& getMatrix() const { return adjacencyMatrix; }

    // Constants for graph representation
    static constexpr int INF = std::numeric_limits<int>::max() / 2; // Prevent overflow in addition

private:
    std::vector<std::vector<int>> adjacencyMatrix;
    size_t numVertices;
};