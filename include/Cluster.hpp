#ifndef A97DC2E7_94AA_4207_9667_07BC5C5517E9
#define A97DC2E7_94AA_4207_9667_07BC5C5517E9

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <deque>
#include "Node.hpp"

/*                              id_source, id_target                 */
using edge_t = std::vector<std::pair<int, int>>;

struct Cluster
{
    edge_t id_nodes;

    size_t upper_bound;
    size_t lower_bound;

    double cluster_cost = 0; /* custo das arestas armazenadas */
    float current_bound = 0; /* custo dos nós ali armazenados */

    bool insertEdge(Node* s_node, Node* t_node, Edge* edge)
    {
        if(current_bound + s_node->weight() + t_node->weight() <= upper_bound)
        {
            std::pair<int, int> temp;
            temp.first = s_node->id();
            temp.second = t_node->id();

            id_nodes.push_back(temp);

            cluster_cost += edge->weight();

            current_bound += s_node->weight() + t_node->weight();

            return true;
        }
        else
        {
            return false;
        }
    };
};

#endif /* A97DC2E7_94AA_4207_9667_07BC5C5517E9 */
