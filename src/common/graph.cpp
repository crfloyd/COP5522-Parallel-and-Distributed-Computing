#include "graph.hpp"
#include <random>
#include <iomanip>

Graph::Graph(const size_t vertices) : numVertices(vertices) {
    if (vertices == 0) {
        throw std::invalid_argument("Graph must have at least one vertex");
    }

    // Initialize matrix with INF, diagonals with 0
    adjacencyMatrix.resize(vertices, std::vector<int>(vertices, INF));
    for (size_t i = 0; i < vertices; ++i) {
        adjacencyMatrix[i][i] = 0;
    }
}

void Graph::setEdge(const size_t src, const size_t dest, const int weight) {
    if (src >= numVertices || dest >= numVertices) {
        throw std::out_of_range("Vertex index out of range");
    }
    adjacencyMatrix[src][dest] = weight;
}

int Graph::getEdge(const size_t src, const size_t dest) const {
    if (src >= numVertices || dest >= numVertices) {
        throw std::out_of_range("Vertex index out of range");
    }
    return adjacencyMatrix[src][dest];
}

void Graph::print() const {
    std::cout << "Distance Matrix:" << std::endl;
    for (size_t i = 0; i < numVertices; ++i) {
        for (size_t j = 0; j < numVertices; ++j) {
            if (adjacencyMatrix[i][j] == INF) {
                std::cout << std::setw(5) << "INF";
            } else {
                std::cout << std::setw(5) << adjacencyMatrix[i][j];
            }
        }
        std::cout << std::endl;
    }
}

Graph Graph::generateRandom(const size_t vertices, const double density, const int minWeight, const int maxWeight) {
    if (density < 0.0 || density > 1.0) {
        throw std::invalid_argument("Density must be between 0 and 1");
    }

    Graph graph(vertices);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> disDensity(0.0, 1.0);
    std::uniform_int_distribution<> disWeight(minWeight, maxWeight);

    for (size_t i = 0; i < vertices; ++i) {
        for (size_t j = 0; j < vertices; ++j) {
            if (i != j && disDensity(gen) < density) {
                graph.setEdge(i, j, disWeight(gen));
            }
        }
    }

    return graph;
}