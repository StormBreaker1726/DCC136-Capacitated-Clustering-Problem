#include "Algorithms.hpp"

Algorithms::Algorithms(std::shared_ptr<Graph> g) {
    this->g = g;
}

double Algorithms::penalization(node_ptr n1, node_ptr n2, size_t cluster_id) {
    double nodes_weight = n1->weight() + n2->weight();
    std::shared_ptr<Cluster> cluster = this->solution->clusters.at(cluster_id);
    double f1 = (nodes_weight) + cluster->current_bound;
    f1 = f1 / cluster->lower_bound;
    double f2 = (nodes_weight) + cluster->current_bound;
    f2 = f2 / cluster->upper_bound;
    /* B*f1 + C*f2 => testar diferentes B e C */
    return 1/4 * f1 + 5 * f2;
}

/* A = 2 (testar para diferentes valores) */
double Algorithms::chance_calc(edge_ptr e, size_t cluster_id) {
    return 2*e->weight() + this->penalization(g->getNode(e->idNode1()), g->getNode(e->idNode2()), cluster_id);
};
sol_ptr Algorithms::greedyHelper(float alpha, size_t iterations) {
    sol_ptr s(new Solution());

    std::vector<Candidate_Edge> cand_list;


}