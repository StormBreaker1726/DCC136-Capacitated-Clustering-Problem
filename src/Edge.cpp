#include "Edge.hpp"

Edge::Edge(int idN1, int idN2, float weight) {
    this->_idNode1 = idN1;
    this->_idNode2 = idN2;
    this->_weight = weight;
}

int Edge::idNode1() {
    return this->_idNode1;
}

int Edge::idNode2() {
    return this->_idNode2;
}

float Edge::weight() {
    return this->_weight;
}