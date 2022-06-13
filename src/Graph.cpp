#include "Graph.hpp"

Graph::Graph(size_t nNode, size_t nCluster, size_t lBound, size_t uBound) {
    this->nNodes = nNode;
    // this->nEdges = nEdge;
    this->nClusters = nCluster;
    this->lowerBound = lBound;
    this->upperBound = uBound;
    this->nodeVector.reserve(nNode);
    // this->edgeVector.reserve(nEdge);
}

void Graph::insertNode(int id, float weight) {
    // sem verificação aqui
    this->nodeVector.push_back(Node(id, weight));
}

void Graph::insertEdge(int id1, int id2, float weight) {
    // sem verificação aqui
    this->edgeVector.push_back(Edge(id1, id2, weight));
}

void Graph::setNEdges(size_t nEdge)
{
    this->nEdges = nEdge;
    this->edgeVector.reserve(nEdge);
}