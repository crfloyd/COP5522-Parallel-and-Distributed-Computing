#pragma once
#include "graph.hpp"
#include <string>

namespace utils {
    Graph readGraphFromFile(const std::string& filename);
    void writeResultToFile(const Graph& result, const std::string& filename);
}