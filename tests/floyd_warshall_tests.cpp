#include <gtest/gtest.h>
#include "common/graph.hpp"
#include <mpi.h>
#include <omp.h>

// Forward declarations (each appears only once)
Graph floydWarshall(const Graph& graph);
Graph floydWarshallOpenMP(const Graph& graph, int numThreads = 0);
Graph floydWarshallMPI(const Graph& graph, int rank, int size);

class FloydWarshallTest : public ::testing::Test {
protected:
    Graph floydWarshall(const Graph& graph) {
        size_t V = graph.size();
        Graph result = graph;
        auto& dist = result.getMatrix();

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
};

TEST_F(FloydWarshallTest, SimpleGraph) {
    Graph g(3);
    g.setEdge(0, 1, 4);
    g.setEdge(1, 2, 3);

    Graph result = floydWarshall(g);

    EXPECT_EQ(result.getEdge(0, 2), 7);  // Path 0->1->2
}

TEST_F(FloydWarshallTest, DisconnectedGraph) {
    Graph g(3);
    g.setEdge(0, 1, 4);

    Graph result = floydWarshall(g);

    EXPECT_EQ(result.getEdge(0, 2), Graph::INF);
}

TEST_F(FloydWarshallTest, OpenMPMatchesSerial) {
    Graph g(5);
    g.setEdge(0, 1, 4);
    g.setEdge(1, 2, 3);
    g.setEdge(2, 3, 2);
    g.setEdge(3, 4, 1);

    Graph serialResult = floydWarshall(g);
    Graph parallelResult = floydWarshallOpenMP(g, 4);

    for (size_t i = 0; i < g.size(); ++i) {
        for (size_t j = 0; j < g.size(); ++j) {
            EXPECT_EQ(serialResult.getEdge(i, j), parallelResult.getEdge(i, j));
        }
    }
}

TEST_F(FloydWarshallTest, OpenMPPerformance) {
    size_t size = 200;
    Graph g = Graph::generateRandom(size, 0.3);

    auto start = std::chrono::high_resolution_clock::now();
    Graph serialResult = floydWarshall(g);
    auto serialDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    start = std::chrono::high_resolution_clock::now();
    Graph parallelResult = floydWarshallOpenMP(g, 4);
    auto parallelDuration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - start).count();

    EXPECT_LT(parallelDuration, serialDuration);
}

TEST_F(FloydWarshallTest, MPIMatchesSerial) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Graph g(5);
    g.setEdge(0, 1, 4);
    g.setEdge(1, 2, 3);
    g.setEdge(2, 3, 2);
    g.setEdge(3, 4, 1);

    Graph serialResult = floydWarshall(g);
    Graph mpiResult = floydWarshallMPI(g, rank, size);

    if (rank == 0) {  // Only check on root process
        for (size_t i = 0; i < g.size(); ++i) {
            for (size_t j = 0; j < g.size(); ++j) {
                EXPECT_EQ(serialResult.getEdge(i, j), mpiResult.getEdge(i, j));
            }
        }
    }
}

TEST_F(FloydWarshallTest, CyclicGraph) {
    Graph g(4);
    g.setEdge(0, 1, 3);
    g.setEdge(1, 2, 2);
    g.setEdge(2, 3, 1);
    g.setEdge(3, 0, 4);

    Graph result = floydWarshall(g);

    EXPECT_EQ(result.getEdge(0, 2), 5);  // Path 0->1->2
    EXPECT_EQ(result.getEdge(1, 0), 7);  // Path 1->2->3->0
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    MPI_Finalize();
    return result;
}