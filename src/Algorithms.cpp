#include "Algorithms.hpp"

Algorithms::Algorithms(std::shared_ptr<Graph> g) {
    this->g = g;
    sol_ptr s(new Solution(g->getNumberClusters(), g->getUpperBound(), g->getLowerBound()));
    this->solution = std::move(s);
}

double Algorithms::penalization(node_ptr n1, node_ptr n2, size_t cluster_id) {
    double nodes_weight = n1->weight() + n2->weight();
    std::shared_ptr<Cluster> cluster = this->solution->clusters.at(cluster_id);
    double f1 = (nodes_weight) + cluster->current_bound;
    if (cluster->lower_bound == 0) {
        f1 = 2 * f1 / cluster->upper_bound;
    } else {
        f1 = f1 / cluster->lower_bound;
    }
    double f2 = (nodes_weight) + cluster->current_bound;
    f2 = f2 / cluster->upper_bound;
    /* B*f1 + C*f2 => testar diferentes B e C */
    return 1/4 * f1 + 5 * f2;
}

/* A = 2 (testar para diferentes valores) */
double Algorithms::chance_calc(edge_ptr e, size_t cluster_id) {
    return 2*e->weight() + this->penalization(g->getNode(e->idNode1()), g->getNode(e->idNode2()), cluster_id);
}



sol_ptr Algorithms::greedyHelper(float alpha) {
    std::vector<Candidate_Edge> cand_list;
    cand_list.reserve(this->g->getNumberEdges() * this->g->getNumberClusters());
    /* Marcar quais nós não foram inseridos */
    std::vector<node_ptr> nodes;
    nodes.reserve(this->g->getNumberNodes());
    nodes = this->g->getNodeVector();

    for (size_t i=0; i<this->g->getNumberEdges(); ++i) {
        for (size_t cluster_id=0; cluster_id<this->g->getNumberClusters(); ++cluster_id) {
            Candidate_Edge c_temp(this->g->getEdgeVector().at(i)->idNode1(), 
                this->g->getEdgeVector().at(i)->idNode2(), cluster_id,
                this->g->getNode(this->g->getEdgeVector().at(i)->idNode1())->weight(),
                this->g->getNode(this->g->getEdgeVector().at(i)->idNode2())->weight(),
                this->chance_calc(this->g->getEdge(
                    this->g->getEdgeVector().at(i)->idNode1(),
                    this->g->getEdgeVector().at(i)->idNode2()),
                cluster_id), true);
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

        /* Se o cliente foi inserido, faz todas as atualizações 
        se não, retira da lista de candidatos */
        if (this->solution->insert_edge_on_cluster(c.cluster_id,
            this->g->getNode(c.s_id),
            this->g->getNode(c.t_id),
            this->g->getEdge(c.s_id, c.t_id))) 
        {
            for (size_t i=0; i<nodes.size(); ++i) {
                if (nodes.at(i)->id() == c.s_id || nodes.at(i)->id() == c.t_id) {
                    nodes.erase(nodes.begin() + i);
                    --i;
                }
            }
            /* Refazer a lista de candidatos */
            /* retirar a aresta inserida na solução */
            size_t cluster = c.cluster_id;
            cand_list.erase(cand_list.begin() + cand_n);
            for (size_t i=0; i<cand_list.size(); ++i) {
                /* recalcular a chance de inserção de cada aresta */
                cand_list.at(i).insertion_chance = this->chance_calc(
                    this->g->getEdge(cand_list.at(i).s_id, cand_list.at(i).t_id),
                    cand_list.at(i).cluster_id);
                /* retirar aresta dos outros clusters */
                if (cand_list.at(i).s_id == c.s_id && cand_list.at(i).t_id == c.t_id) {
                    cand_list.erase(cand_list.begin() + i);
                    --i;
                }
                /* todas as arestas que estão ligadas ao cluster inserido
                devem ser mantidos; aqueles que correspondem a um cluster 
                diferente serão retirados */
                else if (cand_list.at(i).s_id == c.s_id || cand_list.at(i).t_id == c.t_id) {
                    if (cand_list.at(i).cluster_id != cluster) {
                        cand_list.erase(cand_list.begin() + i);
                        --i;
                    }
                }
            }
        } else {
            cand_list.erase(cand_list.begin() + cand_n);
        }
    }
    /* adicionar os nós que faltam nos clusters */
    std::sort(nodes.begin(), nodes.end(), 
        [](node_ptr& a, node_ptr& b) {
            return a->weight() > b->weight();
        });
    for (size_t i=0; i<nodes.size(); ++i) {
        this->solution->insert_node_on_cluster(this->solution->find_minimum_bound_cluster(), nodes.at(i));
    }

    return this->solution;
}

sol_ptr Algorithms::greedyCheaperHelper(float alpha)
{
    std::vector<Candidate_Edge> cand_list;
    cand_list.reserve(g->getNumberEdges());
    std::vector<Candidate_Edge> cand_list_helper;
    cand_list_helper.reserve(g->getNumberEdges());

    for (size_t i=0; i<this->g->getNumberEdges(); ++i) {
        Candidate_Edge c_temp(this->g->getEdgeVector().at(i)->idNode1(), 
                this->g->getEdgeVector().at(i)->idNode2(), 0,
                this->g->getNode(this->g->getEdgeVector().at(i)->idNode1())->weight(),
                this->g->getNode(this->g->getEdgeVector().at(i)->idNode2())->weight(),
                this->chance_calc(this->g->getEdge(
                    this->g->getEdgeVector().at(i)->idNode1(),
                    this->g->getEdgeVector().at(i)->idNode2()),
                0), false);
            cand_list.push_back(c_temp);
    }

    std::sort(cand_list.begin(), cand_list.end(), 
            [](Candidate_Edge& a, Candidate_Edge& b) {
                return a.insertion_chance > b.insertion_chance;
            });

    for (size_t i=0; i<this->g->getNumberClusters(); ++i) {
        Candidate_Edge c = cand_list.at(0);
        this->solution->insert_edge_on_cluster(i,
            this->g->getNode(c.s_id),
            this->g->getNode(c.t_id),
            this->g->getEdge(c.s_id, c.t_id));
        cand_list.erase(cand_list.begin() + 0);
    }

    while (!cand_list.empty()) {
        /* Atualizar lista de candidatos (muito dificil de explicar,
        confia no processo) */
        for (size_t i=0; i<cand_list.size(); ++i) {
            /* Verificar se a aresta possui vértices em nós pertencentes a
            clusters diferentes */
            Candidate_Edge c = cand_list.at(i);
            if (!c.viable) {
                int cluster_1 = this->solution->getNodeCluster(c.s_id);
                int cluster_2 = this->solution->getNodeCluster(c.t_id);
                /* Ambos os nós adjacentes já pertencem a algum cluster */
                if (cluster_1 != -1 && cluster_2 != -1) {
                    /* Adicionar a aresta apenas melhora a solução */
                    if (cluster_1 == cluster_2) {
                        this->solution->insert_edge_on_cluster(
                            (size_t) cluster_1,
                            this->g->getNode(c.s_id),
                            this->g->getNode(c.t_id),
                            this->g->getEdge(c.s_id, c.t_id));
                    }
                    cand_list.erase(cand_list.begin() + i);
                    --i;
                } 
                else if (!(cluster_1 == -1 && cluster_2 == -1)) {
                    /* Procura a aresta ligada a um nó pertencente a um cluster */
                    if (cluster_1 == -1) {
                        c.cluster_id = (size_t) cluster_2;
                    } else {
                        c.cluster_id = (size_t) cluster_1;
                    }
                    c.insertion_chance = this->chance_calc(
                        this->g->getEdge(c.s_id, c.t_id), c.cluster_id);
                    c.viable = true;
                    cand_list_helper.push_back(c);
                }
            }
        }
        
        if (cand_list_helper.empty()) {
            break;
        }

        std::sort(cand_list_helper.begin(), cand_list_helper.end(), 
            [](Candidate_Edge& a, Candidate_Edge& b) {
                return a.insertion_chance > b.insertion_chance;
            });

        while (!cand_list_helper.empty()) {
            size_t nElmts = alpha * cand_list_helper.size();
            size_t cand_n = nElmts == 0 ? 0 : rand() % nElmts;
            Candidate_Edge c = cand_list_helper.at(cand_n);

            /* Se o cliente foi inserido, faz todas as atualizações 
            se não, retira da lista de candidatos */
            if(!this->solution->insert_edge_on_cluster(c.cluster_id,
                this->g->getNode(c.s_id),
                this->g->getNode(c.t_id),
                this->g->getEdge(c.s_id, c.t_id))) 
            {
                cand_list_helper.erase(cand_list_helper.begin() + cand_n);
            } else {
                cand_list_helper.clear();  
            }
        }
    }

    return this->solution;
}

sol_ptr Algorithms::greedy() {
    return this->greedyHelper(0);
}

sol_ptr Algorithms::greedyCheaper() {
    return this->greedyCheaperHelper(0);
}