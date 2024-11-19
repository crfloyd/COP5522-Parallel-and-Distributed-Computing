#include <gtest/gtest.h>
#include "common/graph.hpp"
#include <stdexcept>

class GraphTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

// Test graph construction
TEST_F(GraphTest, ConstructorCreatesValidGraph) {
    Graph g(5);
    EXPECT_EQ(g.size(), 5);

    // Check that diagonal elements are 0 and others are INF
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            if (i == j) {
                EXPECT_EQ(g.getEdge(i, j), 0);
            } else {
                EXPECT_EQ(g.getEdge(i, j), Graph::INF);
            }
        }
    }
}

// Test invalid construction
TEST_F(GraphTest, ConstructorThrowsOnZeroSize) {
    EXPECT_THROW(Graph(0), std::invalid_argument);
}

// Test edge setting and getting
TEST_F(GraphTest, EdgeSetAndGet) {
    Graph g(3);
    g.setEdge(0, 1, 5);
    EXPECT_EQ(g.getEdge(0, 1), 5);
    EXPECT_EQ(g.getEdge(1, 0), Graph::INF); // Directed graph
}

// Test edge bounds checking
TEST_F(GraphTest, EdgeBoundsChecking) {
    Graph g(3);
    EXPECT_THROW(g.setEdge(3, 0, 5), std::out_of_range);
    EXPECT_THROW(g.setEdge(0, 3, 5), std::out_of_range);
    EXPECT_THROW(g.getEdge(3, 0), std::out_of_range);
    EXPECT_THROW(g.getEdge(0, 3), std::out_of_range);
}

// Test random graph generation
TEST_F(GraphTest, RandomGraphGeneration) {
    size_t size = 10;
    double density = 0.3;
    Graph g = Graph::generateRandom(size, density);

    // Check size
    EXPECT_EQ(g.size(), size);

    // Check diagonal elements are 0
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(g.getEdge(i, i), 0);
    }

    // Count non-INF edges to verify approximate density
    int edgeCount = 0;
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < size; ++j) {
            if (i != j && g.getEdge(i, j) != Graph::INF) {
                edgeCount++;
            }
        }
    }

    // Calculate actual density (excluding diagonal elements)
    double actualDensity = static_cast<double>(edgeCount) / (size * (size - 1));
    // Allow for some random variation (within 20%)
    EXPECT_NEAR(actualDensity, density, 0.2);
}

// Test edge weight constraints
TEST_F(GraphTest, EdgeWeightConstraints) {
    Graph g = Graph::generateRandom(5, 0.5, 1, 10);

    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            int weight = g.getEdge(i, j);
            if (i != j && weight != Graph::INF) {
                EXPECT_GE(weight, 1);
                EXPECT_LE(weight, 10);
            }
        }
    }
}

// Test matrix access
TEST_F(GraphTest, MatrixAccess) {
    Graph g(3);
    g.setEdge(0, 1, 5);

    // Test const access
    const Graph& constG = g;
    const auto& constMatrix = constG.getMatrix();
    EXPECT_EQ(constMatrix[0][1], 5);

    // Test non-const access
    auto& matrix = g.getMatrix();
    matrix[1][2] = 3;
    EXPECT_EQ(g.getEdge(1, 2), 3);
}