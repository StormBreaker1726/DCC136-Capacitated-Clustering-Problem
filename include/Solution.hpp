#ifndef D5A7BFE3_EE51_4CC7_9A59_A52FA8FCD518
#define D5A7BFE3_EE51_4CC7_9A59_A52FA8FCD518

#include "defines.hpp"
#include "Cluster.hpp"

using cluster_t = std::vector<std::shared_ptr<Cluster>>;

struct Solution
{
    cluster_t clusters;

    double solution_cost = 0;

    Solution();

    void update_cost()
    {
        for (size_t i = 0; i < clusters.size(); i++)
        {
            solution_cost += clusters.at(i)->cluster_cost;
        }
    };

    void insert_edge_on_cluster(size_t cluster_id, node_ptr n1, node_ptr n2, edge_ptr edge)
    {
        this->clusters.at(cluster_id)->insertEdge(n1, n2, edge);
        this->update_cost();
    };
};


#endif /* D5A7BFE3_EE51_4CC7_9A59_A52FA8FCD518 */
