#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "defines.hpp"
#include "Node.hpp"
#include "Edge.hpp"
#include "Solution.hpp"

class Graph {
    private:
        size_t nNodes;
        size_t nEdges;
        size_t nClusters;
        size_t lowerBound;
        size_t upperBound;
        std::vector<std::shared_ptr<Node>> nodeVector;
        std::vector<std::shared_ptr<Edge>> edgeVector;
    public:
        Graph(size_t nNodes, size_t nClusters, size_t lBound, size_t uBound);
        void insertNode(int id, float weight);
        void insertEdge(int idNode1, int idNode2, float weight);
        void setNEdges(size_t nEdge);
        std::shared_ptr<Edge> getEdge(int s_id, int t_id);
        std::shared_ptr<Node> getNode(int id);
};

#endif // GRAPH_HPP