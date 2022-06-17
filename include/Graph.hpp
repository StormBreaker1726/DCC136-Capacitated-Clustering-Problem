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
        double lowerBound;
        double upperBound;
        std::vector<node_ptr> nodeVector;
        std::vector<edge_ptr> edgeVector;
    public:
        Graph(size_t nNodes, size_t nClusters, double lBound, double uBound);
        void insertNode(int id, float weight);
        void insertEdge(int idNode1, int idNode2, float weight);
        void setNEdges(size_t nEdge);
        size_t getNumberNodes();
        size_t getNumberEdges();
        size_t getNumberClusters();
        double getUpperBound();
        double getLowerBound();
        std::vector<node_ptr> getNodeVector();
        std::vector<edge_ptr> getEdgeVector();
        std::shared_ptr<Edge> getEdge(int s_id, int t_id);
        std::shared_ptr<Node> getNode(int id);
};

#endif // GRAPH_HPP