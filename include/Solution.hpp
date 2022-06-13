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
};


#endif /* D5A7BFE3_EE51_4CC7_9A59_A52FA8FCD518 */
