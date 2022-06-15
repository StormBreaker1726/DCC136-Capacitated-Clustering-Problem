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
    cand_list.reserve(this->g->getNumberEdges() * this->g->getNumberClusters());

    for (size_t i=0; i<this->g->getNumberEdges(); ++i) {
        for (size_t cluster_id=0; cluster_id<this->g->getNumberClusters(); ++cluster_id) {
            Candidate_Edge c_temp(this->g->getEdgeVector().at(i)->idNode1(), 
                this->g->getEdgeVector().at(i)->idNode2(), cluster_id,
                this->g->getNode(this->g->getEdgeVector().at(i)->idNode1())->weight(),
                this->g->getNode(this->g->getEdgeVector().at(i)->idNode2())->weight(),
                this->chance_calc(this->g->getEdge(
                    this->g->getEdgeVector().at(i)->idNode1(),
                    this->g->getEdgeVector().at(i)->idNode2()),
                cluster_id));
            cand_list.push_back(c_temp);
        }
    }
    
    while (!cand_list.empty()) {
        /* sort candidate vector */
        std::sort(cand_list.begin(), cand_list.end(), 
            [](Candidate_Edge& a, Candidate_Edge& b) {
                return a.insertion_chance > b.insertion_chance;
            });
        
        size_t nElmts = alpha * cand_list.size();
        size_t cand_n = nElmts == 0 ? 0 : rand() % nElmts;
        Candidate_Edge c = cand_list.at(cand_n);

        s->insert_edge_on_cluster(c.cluster_id,
            this->g->getNode(c.s_id),
            this->g->getNode(c.t_id),
            this->g->getEdge(c.s_id, c.t_id));
        
        /* Refazer a lista de candidatos */
    }

}