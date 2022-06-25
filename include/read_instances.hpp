#ifndef FBCBAACF_3699_4CDA_9FC8_E1FFED46D735
#define FBCBAACF_3699_4CDA_9FC8_E1FFED46D735

#include "Graph.hpp"

std::shared_ptr<Graph> read_handover(std::ifstream& instance_file)
{
    size_t number_of_nodes;
    size_t number_of_clusters;
    double lower_bounds = 0;
    double upper_bounds;

    std::string garbage;

    //int target_id;
    //int source_id;
    float node_weight;

    int edge_weight; 

    std::vector<std::vector<int>> adjacency_matrix;

    instance_file >> number_of_nodes;
    instance_file >> number_of_clusters;
    instance_file >> upper_bounds;
    //instance_file >> lower_bounds;

    std::shared_ptr<Graph> graph_to_operate(new Graph(number_of_nodes, number_of_clusters, lower_bounds, upper_bounds));

    for (size_t i = 0; i < number_of_nodes; i++)
    {
        instance_file >> node_weight;
        graph_to_operate->insertNode(i, node_weight);
    }

    for (size_t i = 0; i < number_of_nodes; i++)
    {
        std::vector<int> temp;
        for (size_t j = 0; j < number_of_nodes; j++)
        {
            instance_file >> edge_weight;
            temp.push_back(edge_weight);
        }
        adjacency_matrix.push_back(temp);
    }

    for (size_t i = 0; i < number_of_nodes; i++)
    {
        for (size_t j = i; j < number_of_nodes; j++)
        {
            if(adjacency_matrix.at(i).at(j) != 0)
            {
                graph_to_operate->insertEdge(i, j, adjacency_matrix.at(i).at(j));
                graph_to_operate->getNode(i)->insert_edge(j, adjacency_matrix.at(i).at(j));
                graph_to_operate->getNode(j)->insert_edge(i, adjacency_matrix.at(i).at(j));
            }
        }
    }
    std::cout<<"Instância Lida"<<std::endl;
    return graph_to_operate;
}

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
    int number_of_edges = 0;
    while (!instance_file.eof())
    {
        instance_file >> source_id;
        instance_file >> target_id;
        instance_file >> node_weight;
        number_of_edges++;
        graph_to_operate->insertEdge(source_id, target_id, node_weight);
        graph_to_operate->getNode(source_id)->insert_edge(target_id, node_weight);
        graph_to_operate->getNode(target_id)->insert_edge(source_id, node_weight);
    }

    graph_to_operate->setNEdges(number_of_edges);
    std::cout<<"Instância Lida"<<std::endl;
    return graph_to_operate;
}


#endif /* FBCBAACF_3699_4CDA_9FC8_E1FFED46D735 */
