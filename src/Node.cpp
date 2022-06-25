#include "Node.hpp"

Node::Node(int id, float weight) {
    this->_id = id;
    this->_weight = weight;
}

int Node::id() {
    return this->_id;
}

float Node::weight() {
    return this->_weight;
}

void Node::insert_edge(int id_t, double weight) {
    this->_edges.insert({id_t, weight});
}

double Node::edge_weight(int id_t) {
    std::map<int, double>::iterator it = this->_edges.find(id_t);
    if (it != this->_edges.end()) {
        return it->second;
    }
    return 0;
}