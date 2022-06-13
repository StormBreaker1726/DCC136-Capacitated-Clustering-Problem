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
    this->nodeVector.push_back(std::make_shared<Node>(id, weight));
}

void Graph::insertEdge(int id1, int id2, float weight) {
    // sem verificação aqui
    this->edgeVector.push_back(std::make_shared<Edge>(id1, id2, weight));
}

void Graph::setNEdges(size_t nEdge)
{
    this->nEdges = nEdge;
    this->edgeVector.reserve(nEdge);
}

std::shared_ptr<Edge> Graph::getEdge(int s_id, int t_id) {
    for (size_t i=0; i<this->edgeVector.size(); ++i) {
        std::shared_ptr<Edge> e(this->edgeVector.at(i));
        if (e->idNode1() == s_id && e->idNode2() == t_id) {
            return this->edgeVector.at(i);
        }
        if (e->idNode1() == t_id && e->idNode2() == s_id) {
            return this->edgeVector.at(i);
        }
    }
    return nullptr;
}

std::shared_ptr<Node> Graph::getNode(int id) {
    for (size_t i=0; i<this->nodeVector.size(); ++i) {
        std::shared_ptr<Node> n(this->nodeVector.at(i));
        if (n->id() == id) {
            return n;
        }
    }
    return nullptr;
}