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

    double actual_cost() {
        double cost = 0;
        for (size_t i = 0; i < clusters.size(); i++)
        {
            cost += clusters.at(i)->cluster_cost;
        }
        return cost;
    }

    void update_cost()
    {
        this->solution_cost = 0;
        for (size_t i = 0; i < clusters.size(); i++)
        {
            double cost = clusters.at(i)->cluster_cost;
            if (this->clusters.at(i)->lower_bound > this->clusters.at(i)->current_bound) {
                cost = cost - cost/4;
            } else if (this->clusters.at(i)->upper_bound < this->clusters.at(i)->current_bound) {
                cost = cost/2;
            }

            this->solution_cost += cost;
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

    void print_solution(std::ofstream& result)
    {
        for (size_t i = 0; i < this->clusters.size(); i++)
        {
            result << "Cluster " << i << ":\n";
            result << "\t[ " << this->clusters.at(i)->lower_bound <<
                " - " << this->clusters.at(i)->upper_bound << " ] ";
            result << this->clusters.at(i)->current_bound << "\n"; 
            for (size_t j = 0; j < this->clusters.at(i)->id_nodes.size(); j++)
            {
                result<<"\tAresta de "<<this->clusters.at(i)->id_nodes.at(j).first<<" para "<<this->clusters.at(i)->id_nodes.at(j).second<<" "<<std::endl;
            }
            result<<"\tCusto = "<<this->clusters.at(i)->cluster_cost<<"\n";
        }
    }

    bool solutionViable() {
        for (size_t i=0; i<this->clusters.size(); ++i) {
            if (this->clusters.at(i)->upper_bound < this->clusters.at(i)->current_bound) {
                return false;
            }
            if (this->clusters.at(i)->lower_bound > this->clusters.at(i)->current_bound) { 
                return false;
            }
        }
        return true;
    }

    void clean_solution() {
        for (size_t i=0; i<this->clusters.size(); ++i) {
            this->clusters.at(i)->id_nodes.clear();
            this->clusters.at(i)->current_bound = 0;
            this->clusters.at(i)->cluster_cost = 0;
        }
        this->solution_cost = 0;
    }

    std::shared_ptr<Solution> clone() {
        std::shared_ptr<Solution> s(new Solution(this->clusters.size(),
            this->clusters.at(0)->upper_bound,
            this->clusters.at(0)->lower_bound));
        s->solution_cost = this->solution_cost;
        for (size_t id_c = 0; id_c < this->clusters.size(); ++id_c) {
            this->clusters.at(id_c)->copy_into(s->clusters.at(id_c));
        }
        return s;
    }

    void printNodes() {
        std::ofstream of;
        of.open("./solution/vertices.txt");
        if (!of.is_open()) {
            std::cerr << "N??o foi poss??vel abrir o arquivo.";
            return;
        }
        of << "S = {";
        for (size_t i=0; i<this->clusters.size(); ++i) {
            std::vector<int> nodes = this->clusters.at(i)->getNodes();
            of << "{ ";
            for (size_t j=0; j<nodes.size(); ++j) {
                of << ", " << nodes.at(j);
            }
            of << "}, ";
        }
        of << "}";
    } 
};


#endif /* SOLUTION_HPP */
