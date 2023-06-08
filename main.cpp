#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

struct Edge {
    int source;
    int destination;
    int weight;

    Edge(int src, int dest, int w) : source(src), destination(dest), weight(w) {}
};

struct MSTEdge {
    int parent;
    int weight;

    MSTEdge() : parent(-1), weight(0) {}
    MSTEdge(int p, int w) : parent(p), weight(w) {}
};

struct Compare {
    bool operator()(const Edge& lhs, const Edge& rhs) const {
        return lhs.weight > rhs.weight;
    }
};

void findMinimumSpanningTree(int numVertices, const std::vector<Edge>& edges, std::vector<MSTEdge>& resultSet) {
    std::vector<std::vector<Edge>> adjacencyList(numVertices);

    for (const auto& edge : edges) {
        if (edge.weight >= 0) {
            adjacencyList[edge.source].push_back(edge);
            adjacencyList[edge.destination].push_back(Edge(edge.destination, edge.source, edge.weight));
        }
    }

    std::priority_queue<Edge, std::vector<Edge>, Compare> pq;
    std::vector<bool> visited(numVertices, false);

    visited[0] = true;
    for (const auto& edge : adjacencyList[0]) {
        pq.push(edge);
    }

    while (!pq.empty()) {
        const Edge currentEdge = pq.top();
        pq.pop();

        if (visited[currentEdge.destination]) {
            continue;
        }

        visited[currentEdge.destination] = true;
        resultSet[currentEdge.destination] = MSTEdge(currentEdge.source, currentEdge.weight);

        for (const auto& edge : adjacencyList[currentEdge.destination]) {
            pq.push(edge);
        }
    }
}

int main() {
    std::cerr << "Welcome to MST TestProgram" << endl;
    std::ifstream inputFile("CIS-LAND4.dat");
    if (!inputFile) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    int numVertices, numEdges;
    inputFile >> numVertices >> numEdges;
    if (numEdges <= 0) {
        std::cerr << "It cannot be a connected graph with " << numEdges << " edges." << std::endl;
        return 1;
    }
    if (numVertices < 0) {
        std::cerr << "ERROR: Number of vertices (" << numVertices << ") is less than zero. The empty graph will be created." << std::endl;
        return numVertices;
    }
    if (numVertices == 0) {
        std::cerr << "ERROR: Number of vertices is zero. The empty graph will be created." << std::endl;
        return 1;
    }
    if (numEdges < numVertices - 1) {
        std::cerr << "It cannot be a connected graph." << std::endl;
        return 1;
    }

    std::vector<Edge> edges;
    for (int i = 0; i < numEdges; ++i) {
        int source, destination, weight;
        inputFile >> source >> destination >> weight;
        if (weight < 0) {
            std::cerr << "Invalid weight - cannot add edge: " << source << " " << destination << " " << weight << ". Edge request ignored." << std::endl;
            return 1;
        }
        else {
            edges.emplace_back(source, destination, weight);
        }
    }

    inputFile.close();

    std::vector<MSTEdge> resultSet(numVertices);
    findMinimumSpanningTree(numVertices, edges, resultSet);

    // Print the adjacency list for the full graph
    std::cout << "Full Graph - Adjacency List" << std::endl;
    for (int i = 0; i < numVertices; ++i) {
        std::cout << "Adj[" << i << "] -> ";
        for (const auto& edge : edges) {
            if (edge.source == i && edge.weight >= 0) {
                std::cout << "(" << edge.destination << "," << edge.weight << ") ";
            }
            else if (edge.destination == i && edge.weight >= 0) {
                std::cout << "(" << edge.source << "," << edge.weight << ") ";
            }
        }
        std::cout << std::endl;
    }

    // Print the minimum spanning tree
    std::cout << std::endl;
    std::cout << "Minimum Spanning Tree" << std::endl;
    for (int i = 1; i < numVertices; ++i) {
        std::cout << "Edge: " << i << " - " << resultSet[i].parent << " weight: " << resultSet[i].weight << std::endl;
    }

    // Prompt user for output file name
    std::cout << "Enter output file name: ";
    std::string outputFileName;
    std::cin >> outputFileName;

    // Write results to output file
    std::ofstream outputFile(outputFileName);
    if (!outputFile) {
        std::cerr << "Failed to open output file." << std::endl;
        return 1;
    }

    outputFile << "Full Graph - Adjacency List" << std::endl;
    for (int i = 0; i < numVertices; ++i) {
        outputFile << "Adj[" << i << "] -> ";
        for (const auto& edge : edges) {
            if (edge.source == i && edge.weight >= 0) {
                outputFile << "(" << edge.destination << "," << edge.weight << ") ";
            }
            else if (edge.destination == i && edge.weight >= 0) {
                outputFile << "(" << edge.source << "," << edge.weight << ") ";
            }
        }
        outputFile << std::endl;
    }

    outputFile << std::endl;
    outputFile << "Minimum Spanning Tree" << std::endl;
    for (int i = 1; i < numVertices; ++i) {
        outputFile << "Edge: " << i << " - " << resultSet[i].parent << " weight: " << resultSet[i].weight << std::endl;
    }

    outputFile.close();

    std::cout << "Results have been written to the output file: " << outputFileName << std::endl;

    return 0;
}
