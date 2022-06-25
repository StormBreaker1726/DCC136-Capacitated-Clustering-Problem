#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include "defines.hpp"
#include "Cluster.hpp"

using cluster_t = std::vector<std::shared_ptr<Cluster>>;
using edge_ptr = std::shared_ptr<Edge>;
using node_ptr = std::shared_ptr<Node>;

struct Solution
{
    cluster_t clusters;

    double solution_cost;

    Solution(size_t nClusters, double uBound, double lBound) 
    {
        this->clusters.reserve(nClusters);
        for (size_t i=0; i<nClusters; ++i) {
            std::shared_ptr<Cluster> c(new Cluster(uBound, lBound));
            this->clusters.push_back(c);
        }
        this->solution_cost = 0;
    };

    void update_cost()
    {
        this->solution_cost = 0;
        for (size_t i = 0; i < clusters.size(); i++)
        {
            this->solution_cost += clusters.at(i)->cluster_cost;
        }
    };

    bool insert_edge_on_cluster(size_t cluster_id, node_ptr n1, node_ptr n2, edge_ptr edge)
    {
        if (this->clusters.at(cluster_id)->insertEdge(n1, n2, edge)) {
            this->update_cost();
            return true;
        }
        return false;
    };

    void insert_node_on_cluster(size_t cluster_id, node_ptr n) {
        this->clusters.at(cluster_id)->insertNode(n);
    }

    size_t find_minimum_bound_cluster()
    {
        size_t min_id = 0;
        for (size_t i=0; i<this->clusters.size(); ++i) {
            if (this->clusters.at(i)->current_bound < this->clusters.at(min_id)->current_bound) {
                min_id = i;
            }
        }
        return min_id;
    }

    int getNodeCluster(int id) {
        for (size_t i=0; i<this->clusters.size(); ++i) {
            if (this->clusters.at(i)->findNode(id)) {
                return i;
            }
        }
        return -1;
    }

    void print_solution()
    {
        std::ofstream result("resultado.txt");
        for (size_t i = 0; i < this->clusters.size(); i++)
        {
            result<<"Cluster "<<i<<":"<<std::endl;
            for (size_t j = 0; j < this->clusters.at(i)->id_nodes.size(); j++)
            {
                result<<"\tAresta de "<<this->clusters.at(i)->id_nodes.at(j).first<<" para "<<this->clusters.at(i)->id_nodes.at(j).second<<" "<<std::endl;
            }
            result<<"\tCusto = "<<this->clusters.at(i)->cluster_cost<<"\n";
        }
    }
};


#endif /* SOLUTION_HPP */
