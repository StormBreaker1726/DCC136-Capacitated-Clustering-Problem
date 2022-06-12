#ifndef FBCBAACF_3699_4CDA_9FC8_E1FFED46D735
#define FBCBAACF_3699_4CDA_9FC8_E1FFED46D735

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory.h>
#include "Graph.hpp"

// void read_handover(std::shared_ptr<Graph> graph_to_operate, std::ifstream& instance_file)
// {
//     size_t number_of_nodes;
//     size_t number_of_clusters;
//     double cluster_capacity;

//     float node_weight;

//     size_t i = 0;
//     int id = 0;
//     while (!instance_file.eof())
//     {
//         if(i == 0)
//         {
//             instance_file >> number_of_nodes;
//         }
//         else if(i == 1)
//         {
//             instance_file >> number_of_clusters;
//         }
//         else if(i == 2)
//         {
//             instance_file >> cluster_capacity;
//         }
//         else
//         {
//             instance_file >> node_weight;
//             graph_to_operate->insertNode(id, node_weight);
//         }
//         id++;
//         i++;
//     }

// }

std::shared_ptr<Graph> read_ran_and_sparse(std::ifstream& instance_file)
{
    size_t number_of_nodes;
    size_t number_of_clusters;
    std::vector<int> lower_bounds;
    std::vector<int> upper_bounds;

    std::string garbage;

    int target_id;
    int source_id;
    float node_weight;

    instance_file >> number_of_nodes;
    instance_file >> number_of_clusters;

    instance_file >> garbage;

    for (size_t i = 0; i < number_of_clusters; i++)
    {
        int l_bound;
        int u_bound;

        instance_file >> l_bound;
        instance_file >> u_bound;

        lower_bounds.push_back(l_bound);
        upper_bounds.push_back(u_bound);
    }
    instance_file >> garbage;

    std::shared_ptr<Graph> graph_to_operate(new Graph(number_of_nodes, number_of_clusters, lower_bounds.at(0), upper_bounds.at(0)));

    for (size_t i = 0; i < number_of_nodes; i++)
    {
        instance_file >> node_weight;
        graph_to_operate->insertNode(i, node_weight);
    }

    while (!instance_file.eof())
    {
        instance_file >> source_id;
        instance_file >> target_id;
        instance_file >> node_weight;

        graph_to_operate->insertEdge(source_id, target_id, node_weight);
    }

    return graph_to_operate;
}


#endif /* FBCBAACF_3699_4CDA_9FC8_E1FFED46D735 */
