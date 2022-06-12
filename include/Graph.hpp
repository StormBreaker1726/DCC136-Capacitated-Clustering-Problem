#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <memory>
#include "Node.hpp"
#include "Edge.hpp"

class Graph {
    private:
        size_t nNodes;
        size_t nEdges;
        size_t nClusters;
        size_t lowerBound;
        size_t upperBound;
        std::vector<Node> nodeVector;
        std::vector<Edge> edgeVector;
    public:
        Graph(size_t nNodes, size_t nEdge, size_t nClusters, size_t lBound, size_t uBound);
        void insertNode(int id, float weight);
        void insertEdge(int idNode1, int idNode2, float weight);
};

#endif // GRAPH_HPP