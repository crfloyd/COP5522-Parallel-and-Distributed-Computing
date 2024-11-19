#pragma once
#include <vector>

class Graph {
public:
    Graph(int vertices);
    void setEdge(int from, int to, int weight);
    int getEdge(int from, int to) const;
private:
    std::vector<std::vector<int>> adjacencyMatrix;
    int numVertices;
};