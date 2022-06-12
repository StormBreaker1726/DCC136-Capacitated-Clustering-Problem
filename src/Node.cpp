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