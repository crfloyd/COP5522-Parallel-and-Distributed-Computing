#include "graph.hpp"

Graph::Graph(int vertices) : numVertices(vertices) {
    adjacencyMatrix.resize(vertices, std::vector<int>(vertices, INT_MAX));
    for(int i = 0; i < vertices; i++) {
        adjacencyMatrix[i][i] = 0;
    }
}

void Graph::setEdge(int from, int to, int weight) {
    adjacencyMatrix[from][to] = weight;
}

int Graph::getEdge(int from, int to) const {
    return adjacencyMatrix[from][to];
}