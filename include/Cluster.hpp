#ifndef CLUSTER_HPP
#define CLUSTER_HPP

#include "Node.hpp"

/*                              id_source, id_target                 */
using edge_t = std::vector<std::pair<int, int>>;
using edge_ptr = std::shared_ptr<Edge>;
using node_ptr = std::shared_ptr<Node>;

struct Cluster
{
    edge_t id_nodes;

    double upper_bound;
    double lower_bound;
    double current_bound; /* custo dos nós ali armazenados */

    double cluster_cost; /* custo das arestas armazenadas */

    Cluster(double uBound, double lBound) {
        this->id_nodes.reserve(0);
        this->upper_bound = uBound;
        this->lower_bound = lBound;
        this->cluster_cost = 0;
        this->current_bound = 0;
    }

    void insertEdge(node_ptr s_node, node_ptr t_node, edge_ptr edge)
    {
        double bound = s_node->weight() + t_node->weight();
        for (size_t i=0; i<this->id_nodes.size(); ++i) {
            /* Se o nó já está num cluster */
            if (this->id_nodes.at(i).first == s_node->id() || this->id_nodes.at(i).second == s_node->id()) {
                bound -= s_node->weight();
                break;
            }
        }
        for (size_t i=0; i<this->id_nodes.size(); ++i) {
            /* Se o nó já está num cluster */
            if (this->id_nodes.at(i).first == t_node->id() || this->id_nodes.at(i).second == t_node->id()) {
                bound -= t_node->weight();
                break;
            }
        }
        if(current_bound + bound <= upper_bound)
        {
            std::pair<int, int> temp;
            temp.first = s_node->id();
            temp.second = t_node->id();

            this->id_nodes.push_back(temp);

            this->cluster_cost += edge->weight();

            this->current_bound += bound;

            return;
        }
    };

    void insertNode(node_ptr n)
    {
        /* assumindo que todo nó que chega aqui não pertence a nenhum cluster */
        std::pair<int, int> temp;
        temp.first = n->id();
        temp.second = -1;
        this->current_bound += n->weight();
        id_nodes.push_back(temp);
    }
};

#endif /* CLUSTER_HPP */
