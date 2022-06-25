#ifndef NODE_HPP
#define NODE_HPP

#include "defines.hpp"
#include "Edge.hpp"
class Node
{
    private:
        int _id;
        float _weight;
        std::map<int, double> _edges;
    public:
        Node(int id, float weight);

        int id();
        float weight();
        void insert_edge(int id_t, double weight);
        double edge_weight(int id_t);
};

#endif // NODE_HPP