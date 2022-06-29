#include "Algorithms.hpp"

Algorithms::Algorithms(std::shared_ptr<Graph> g) {
    this->g = g;
    sol_ptr s(new Solution(g->getNumberClusters(), g->getUpperBound(), g->getLowerBound()));
    this->solution = std::move(s);
}

double Algorithms::penalization(node_ptr n1, node_ptr n2, size_t cluster_id) {
    double nodes_weight = 0;
    if (n1 == nullptr) {
        nodes_weight = n2->weight();
    } else if (n2 == nullptr) {
        nodes_weight = n1->weight();
    } else {
        nodes_weight = n1->weight() + n2->weight();
    }
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
    return 1/4 * f1 + this->g->getNumberNodes() * 10 * f2;
}

/* A = 2 (testar para diferentes valores) */
double Algorithms::chance_calc(edge_ptr e, size_t cluster_id) {
    return 2*e->weight() - this->penalization(g->getNode(e->idNode1()), g->getNode(e->idNode2()), cluster_id);
}

void Algorithms::insert_edges_cluster(size_t c_id) {
    size_t n = this->solution->clusters.at(c_id)->id_nodes.size();
    for (size_t i=0; i<n-1 && i<this->solution->clusters.at(c_id)->id_nodes.size()-1; ++i) {
        std::pair<int, int>* v_i = &this->solution->clusters.at(c_id)->id_nodes.at(i);
        for (size_t j=i+1; j<n && j<this->solution->clusters.at(c_id)->id_nodes.size(); ++j) {
            std::pair<int, int>* v_next = &this->solution->clusters.at(c_id)->id_nodes.at(j);
            /* Se tiver aresta entre dois vértices*/
            if (this->g->getNode(v_i->first)->edge_weight(v_next->first) > 0 
                && v_i->first != v_next->second && v_i->second != v_next->first) {
                //this->solution->solution_cost += this->g->getNode(v_i->first)->edge_weight(v_next->first);
                this->solution->clusters.at(c_id)->cluster_cost += this->g->getNode(v_i->first)->edge_weight(v_next->first);
                if (v_next->second == -1) {
                    v_next->second = v_i->first;
                } else if (v_i->second == -1) {
                    v_i->second = v_next->first;
                } else {
                    this->solution->clusters.at(c_id)->id_nodes.push_back({v_i->first, v_next->first});
                    v_i = &this->solution->clusters.at(c_id)->id_nodes.at(i);
                }
                if (v_i->second == -1) {
                    this->solution->clusters.at(c_id)->id_nodes.erase(
                        this->solution->clusters.at(c_id)->id_nodes.begin() + i
                    );
                    if (i>0) --i;
                    if (j>0) --j;
                }
            }
        }
    }
    this->solution->update_cost();
}



void Algorithms::insert_all_edges() {
    for (size_t c_id=0; c_id<this->solution->clusters.size(); ++c_id) {
        size_t n = this->solution->clusters.at(c_id)->id_nodes.size();
        for (size_t i=0; i<n-1 && i<this->solution->clusters.at(c_id)->id_nodes.size()-1; ++i) {
            std::pair<int, int>* v_i = &this->solution->clusters.at(c_id)->id_nodes.at(i);
            for (size_t j=i+1; j<n && j<this->solution->clusters.at(c_id)->id_nodes.size(); ++j) {
                std::pair<int, int>* v_next = &this->solution->clusters.at(c_id)->id_nodes.at(j);
                /* Se tiver aresta entre dois vértices*/
                if (this->g->getNode(v_i->first)->edge_weight(v_next->first) > 0 
                    && v_i->first != v_next->second && v_i->second != v_next->first) {
                    //this->solution->solution_cost += this->g->getNode(v_i->first)->edge_weight(v_next->first);
                    this->solution->clusters.at(c_id)->cluster_cost += this->g->getNode(v_i->first)->edge_weight(v_next->first);
                    if (v_next->second == -1) {
                        v_next->second = v_i->first;
                    } else if (v_i->second == -1) {
                        v_i->second = v_next->first;
                    } else {
                        this->solution->clusters.at(c_id)->id_nodes.push_back({v_i->first, v_next->first});
                        v_i = &this->solution->clusters.at(c_id)->id_nodes.at(i);
                    }
                    if (v_i->second == -1) {
                        this->solution->clusters.at(c_id)->id_nodes.erase(
                            this->solution->clusters.at(c_id)->id_nodes.begin() + i
                        );
                        if (i>0) --i;
                        if (j>0) --j;
                    }
                }
            }
        }
    }
    this->solution->update_cost();
}

void Algorithms::insert_edges_cluster2(size_t c_id) {
    std::vector<int> nodes = this->solution->clusters.at(c_id)->getNodes();
    double cost = 0;
    for (size_t i=0; i<nodes.size(); ++i) {
        node_ptr node = this->g->getNode(nodes.at(i));
        for (size_t j=i; j<nodes.size(); ++j) {
            cost += node->edge_weight(nodes.at(j));
        }
    }
    this->solution->clusters.at(c_id)->cluster_cost += cost;
    this->solution->update_cost();
}

void Algorithms::insert_all_edges2() {
    for (size_t i=0; i<this->solution->clusters.size(); ++i) {
        this->insert_edges_cluster2(i);
    }
    this->solution->update_cost();
}

sol_ptr Algorithms::greedyFirstHelper(float alpha) {
    this->solution->clean_solution();
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
        nodes.erase(nodes.begin() + i);
        --i;
    }

    return this->solution;
}

sol_ptr Algorithms::greedyCheaperHelper(float alpha)
{
    this->solution->clean_solution();
    std::vector<Candidate_Edge> cand_list;
    cand_list.reserve(g->getNumberEdges());
    std::vector<Candidate_Edge> cand_list_helper;
    cand_list_helper.reserve(g->getNumberEdges());
    std::vector<node_ptr> nodes;
    nodes.reserve(g->getNumberNodes());
    nodes = g->getNodeVector();

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
        if (nodes.empty()) {
            return this->solution;
        }
        if (cand_list.empty()) {
            this->solution->insert_node_on_cluster(i, nodes.at(0));
            nodes.erase(nodes.begin() + 0);
        } else {
            Candidate_Edge c = cand_list.at(0);
            if (!(this->solution->insert_edge_on_cluster(i,
                this->g->getNode(c.s_id),
                this->g->getNode(c.t_id),
                this->g->getEdge(c.s_id, c.t_id)))) {
                    --i;
            } else {
                for (size_t j=0; j<nodes.size(); ++j) {
                        if (nodes.at(j)->id() == c.s_id || nodes.at(j)->id() == c.t_id) {
                            nodes.erase(nodes.begin() + j);
                            --j;
                        }
                }
            }
            cand_list.erase(cand_list.begin() + 0);
            for (size_t j=0; j<cand_list.size(); ++j) {
                if (cand_list.at(j).s_id == c.s_id ||
                    cand_list.at(j).s_id == c.t_id ||
                    cand_list.at(j).t_id == c.s_id ||
                    cand_list.at(j).t_id == c.t_id) {
                        cand_list.erase(cand_list.begin() + j);
                        --j;    
                    }
            }
        }
    }
    cand_list.clear();
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

    while (!cand_list.empty()) {
        /* Atualizar lista de candidatos (muito dificil de explicar,
        confia no processo) */
        for (size_t i=0; i<cand_list.size(); ++i) {
            /* Verificar se a aresta possui vértices em nós pertencentes a
            clusters diferentes */
            Candidate_Edge* c = &cand_list.at(i);
            //if (!c->viable) {
                int cluster_1 = this->solution->getNodeCluster(c->s_id);
                int cluster_2 = this->solution->getNodeCluster(c->t_id);
                /* Ambos os nós adjacentes já pertencem a algum cluster */
                if (cluster_1 != -1 && cluster_2 != -1) {
                    /* Adicionar a aresta apenas melhora a solução */
                    if (cluster_1 == cluster_2) {
                        this->solution->insert_edge_on_cluster(
                            (size_t) cluster_1,
                            this->g->getNode(c->s_id),
                            this->g->getNode(c->t_id),
                            this->g->getEdge(c->s_id, c->t_id));
                    }
                    cand_list.erase(cand_list.begin() + i);
                    --i;
                } 
                else if (!(cluster_1 == -1 && cluster_2 == -1)) {
                    /* Procura a aresta ligada a um nó pertencente a um cluster */
                    if (cluster_1 == -1) {
                        c->cluster_id = (size_t) cluster_2;
                    } else {
                        c->cluster_id = (size_t) cluster_1;
                    }
                    c->insertion_chance = this->chance_calc(
                        this->g->getEdge(c->s_id, c->t_id), c->cluster_id);
                    c->viable = true;
                    cand_list_helper.push_back(*c);
                }
            //}
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
                if (cand_list_helper.empty()) {
                    cand_list.clear();
                }
            } else {
                for (size_t i=0; i<nodes.size(); ++i) {
                    if (nodes.at(i)->id() == c.s_id || nodes.at(i)->id() == c.t_id) {
                        nodes.erase(nodes.begin() + i);
                        break;
                    }
                }
                cand_list_helper.clear();  
            }
        }
    }

    if (!nodes.empty()) {
        /* adicionar os nós que faltam nos clusters */
        std::sort(nodes.begin(), nodes.end(), 
            [](node_ptr& a, node_ptr& b) {
                return a->weight() > b->weight();
            });
        for (size_t i=0; i<nodes.size(); ++i) {
            this->solution->insert_node_on_cluster(this->solution->find_minimum_bound_cluster(), nodes.at(i));
            nodes.erase(nodes.begin() + i);
            --i;
        }
    }
    this->insert_all_edges2();
    return this->solution;
}

sol_ptr Algorithms::greedyNodesHelper(float alpha) {
    this->solution->clean_solution();
    std::vector<Candidate_Node> cand_list;
    cand_list.reserve(this->g->getNumberNodes());

    /* RISCO DE VIR SOLUÇÃO INVIÁVEL!!! */

    /* cria candidato tentando colocar no melhor cluster atual */ 
    for (size_t i=0; i<this->g->getNumberNodes(); ++i) {
        double bestPen = MAXFLOAT;
        size_t c_id = 0;
        for (size_t j=0; j<this->g->getNumberClusters(); ++j) {
            if (bestPen < this->penalization(this->g->getNodeVector().at(i), nullptr, j)) {
                bestPen = this->penalization(this->g->getNodeVector().at(i), nullptr, j);
                c_id = j;
            }
        }
        Candidate_Node c_node(this->g->getNodeVector().at(i)->id(), bestPen, c_id);
        cand_list.push_back(c_node);
    }


    while (!cand_list.empty()) {
        std::sort(cand_list.begin(), cand_list.end(), 
            [](Candidate_Node& a, Candidate_Node& b) {
                return a.penalization < b.penalization;
            });
        /* Seleciona um candidato aleatório */
        size_t nElmts = alpha * cand_list.size();
        size_t cand_n = nElmts == 0 ? 0 : rand() % nElmts;
        Candidate_Node c = cand_list.at(cand_n);

        this->solution->insert_node_on_cluster(c.cluster_id, this->g->getNode(c.node_id));
        cand_list.erase(cand_list.begin() + cand_n);

        for (size_t i=0; i<cand_list.size(); ++i) {
            double bestPen = MAXFLOAT;
            size_t c_id = cand_list.at(i).cluster_id;
            for (size_t j=0; j<this->g->getNumberClusters(); ++j) {
                double pen = this->penalization(this->g->getNode(c.node_id), nullptr, j);
                if (bestPen > pen) {
                    bestPen = pen;
                    c_id = j;
                }
            }
            cand_list.at(i).cluster_id = c_id;
            cand_list.at(i).penalization = bestPen;
        }
    }
    /* Cria as arestas */
    this->insert_all_edges2();
    return this->solution;
}

sol_ptr Algorithms::greedyFirst() {
    return this->greedyFirstHelper(0);
}

sol_ptr Algorithms::greedyCheaper() {
    return this->greedyCheaperHelper(0);
}

sol_ptr Algorithms::greedyNodes() {
    return this->greedyNodesHelper(0);
}

sol_ptr Algorithms::greedy(float alpha, size_t it) {

    Solution sBest = *this->greedyNodesHelper(0);

    if (this->g->getNumberEdges() > 0) {
        for (size_t i=1; i<it; ++i) {
            Solution s = *this->greedyNodesHelper(alpha);
            if (s.solution_cost > sBest.solution_cost) {
                sBest = *s.clone();
            }
        }
        this->solution = std::make_shared<Solution>(sBest);
        return this->solution;
    }

    for (size_t i=1; i<it; ++i) {
        size_t cand_n = rand() % 2;
        if (cand_n == 0) {
            Solution s = *this->greedyNodesHelper(alpha);
            if (s.solution_cost > sBest.solution_cost) {
                    sBest = *s.clone();
            }
        } else if (cand_n == 1) {
            /* Demora muito - o rand ali não chega aqui */        
            Solution s = *this->greedyFirstHelper(alpha);
            if (s.solution_cost > sBest.solution_cost) {
                    sBest = *s.clone();
            }
        } else {
            Solution s = *this->greedyCheaperHelper(alpha);
            if (s.solution_cost > sBest.solution_cost) {
                    sBest = *s.clone();
            }
        }
    }
    this->solution = std::make_shared<Solution>(sBest);
    return this->solution;
}

sol_ptr Algorithms::destruction(Solution& s, std::vector<int>& nodes) {
    // escolher dois clusters:
    size_t escolha = rand() % 3;
    
    if (escolha == 0) {
    // 1: Escolher o mais pesado com o mais leve:
        size_t id_leve = 0;
        size_t id_pesa = 0;
        for (size_t i=0; i<s.clusters.size(); ++i) {
            if (s.clusters.at(i)->current_bound < s.clusters.at(id_leve)->current_bound) {
                id_leve = i;
            }
            else if (s.clusters.at(i)->current_bound > s.clusters.at(id_pesa)->current_bound) {
                id_pesa = i;
            }
        }
        // pegar os vértices de cada cluster
        nodes = s.clusters.at(id_leve)->getNodes();
        do {
            std::vector<int> helper = s.clusters.at(id_pesa)->getNodes();
            nodes.insert(nodes.end(), helper.begin(), helper.end());
        } while (0);

        s.clusters.at(id_leve)->clearCluster();
        s.clusters.at(id_pesa)->clearCluster();

    } else if (escolha == 1) {
    // 2: Escolher os dois mais baratos
        size_t bar1 = 0;
        size_t bar2 = 1;
        for (size_t i=0; i<s.clusters.size(); ++i) {
            if (s.clusters.at(i)->cluster_cost < s.clusters.at(bar1)->cluster_cost) {
                bar2 = bar1;
                bar1 = i;
            } else if (s.clusters.at(i)->cluster_cost < s.clusters.at(bar2)->cluster_cost
                && i != bar1) {
                bar2 = i;
            }
        }
        nodes = s.clusters.at(bar1)->getNodes();
        do {
            std::vector<int> helper = s.clusters.at(bar2)->getNodes();
            nodes.insert(nodes.end(), helper.begin(), helper.end());
        } while (0);
        s.clusters.at(bar1)->clearCluster();
        s.clusters.at(bar2)->clearCluster();
    } else {
    // 3: Escolher dois aleatórios
        do {
            int c1 = rand() % s.clusters.size();
            int c2 = -1;
            do {
                c2 = rand() % s.clusters.size();
            } while (c2 == c1);

            nodes = s.clusters.at(c1)->getNodes();
            s.clusters.at(c1)->clearCluster();
            std::vector<int> vec_help = s.clusters.at(c2)->getNodes();
            nodes.insert(nodes.end(), vec_help.begin(), vec_help.end());
            s.clusters.at(c2)->clearCluster();
        } while (0);
    } 

    return std::make_shared<Solution>(s);
}

sol_ptr Algorithms::construction(float alpha, Solution& s, std::vector<int>& nodes) {
    
    std::vector<Candidate_Node> cand_list;
    cand_list.reserve(nodes.size());
    /* Iterar pelos vértices fora de algum dos 2 clusters; 
        Ver qual é a melhor opção de inserção para ele. 
        Ordenar os candidatos e escolher o N melhor */
    
    // encontrar os dois clusters vazios
    int c1 = -1;
    int c2 = -1;
    for (size_t i=0; i<s.clusters.size(); ++i) {
        if (s.clusters.at(i)->id_nodes.empty()) {
            if (c1 != -1) {
                c2 = i;
            } else {
                c1 = i;
            }
        }
    }
    // inicializa os dois clusters com vértices aleatórios
    do {
        int n1 = rand() % nodes.size();
        int n2 = -1;
        do {
            n2 = rand() % nodes.size();
        } while (n2 == n1);
        s.clusters.at(c1)->insertNode(this->g->getNode(*(nodes.begin() + n1)));
        s.clusters.at(c2)->insertNode(this->g->getNode(*(nodes.begin() + n2)));

        if (n1 > n2) {
            nodes.erase(nodes.begin()+n1);
            nodes.erase(nodes.begin()+n2);
        } else {
            nodes.erase(nodes.begin()+n2);
            nodes.erase(nodes.begin()+n1);    
        }
    } while (0);
    // inicializa a lista de candidatos
    for (size_t i=0; i<nodes.size(); ++i) {
        double pen1 = MAXFLOAT;
        double pen2 = MAXFLOAT;
        node_ptr node = this->g->getNode(nodes.at(i));
        if (node->edge_weight(s.clusters.at(c1)->id_nodes.at(0).first) > 0) {
            pen1 = this->penalization(node, nullptr, c1);
        }
        if (node->edge_weight(s.clusters.at(c2)->id_nodes.at(0).first) > 0) {
            pen2 = this->penalization(node, nullptr, c2);
        }
        if (pen1 < pen2) {
            Candidate_Node c_node(node->id(), pen1, c1);
            cand_list.push_back(c_node);
        } else {
            Candidate_Node c_node(node->id(), pen2, c2);
            cand_list.push_back(c_node);
        }
    }
    nodes.clear();
    while (!cand_list.empty()) {

        for (size_t i=0; i<cand_list.size(); i++) {
            double custo1 = 0;
            double custo2 = 0;
            node_ptr node = this->g->getNode(cand_list.at(i).node_id);
            for (size_t j=0; j<s.clusters.at(c1)->id_nodes.size(); ++j) {
                custo1 += node->edge_weight(s.clusters.at(c1)->id_nodes.at(j).first);
            }
            for (size_t j=0; j<s.clusters.at(c2)->id_nodes.size(); ++j) {
                custo2 += node->edge_weight(s.clusters.at(c2)->id_nodes.at(j).first);
            }
            custo1 -= this->penalization(node, nullptr, c1);
            custo2 -= this->penalization(node, nullptr, c2);
            if (custo1 > custo2) {
                cand_list.at(i).cluster_id = c1;
                cand_list.at(i).penalization = custo1;
            } else {
                cand_list.at(i).cluster_id = c2;
                cand_list.at(i).penalization = custo2;
            }
        }

        std::sort(cand_list.begin(), cand_list.end(), 
            [](Candidate_Node& a, Candidate_Node& b) {
                return a.penalization > b.penalization;
            });
        /* Seleciona um candidato aleatório */
        size_t nElmts = alpha * cand_list.size();
        size_t cand_n = nElmts == 0 ? 0 : rand() % nElmts;
        Candidate_Node c = cand_list.at(cand_n);

        this->solution->insert_node_on_cluster(c.cluster_id, this->g->getNode(c.node_id));
        cand_list.erase(cand_list.begin() + cand_n);
    }

    this->solution = std::make_shared<Solution>(s);
    this->insert_edges_cluster2(c1);
    this->insert_edges_cluster2(c2);
    return this->solution;
}

sol_ptr Algorithms::iteratedGreedy(float alpha, size_t it, tempo_t inicio) {
    std::vector<int> nodes;
    nodes.reserve(this->g->getNumberNodes());
    
    Solution s = *this->greedy(alpha, it);
    Solution sBest = *s.clone();

    std::cout << "\nSolução Inicial Gerada\n";

    size_t no_improv = 0;
    while (true) {
        /* DESTROI 2 CLUSTERS */
        s = *this->destruction(s, nodes);

        /* REMONTA OS CLUSTERS */
        s = *this->construction(alpha, s, nodes);
        
        if (s.solution_cost > sBest.solution_cost) {
            sBest = *s.clone();
            no_improv = 0;
        } else {
            /* sem melhora */
            ++no_improv;
        }
        if (no_improv > this->g->getNumberNodes()*100 || no_improv > 800) {
            break;
        }
        tempo_t agora = relogio_t::now();
        if (std::chrono::duration_cast<std::chrono::seconds>((agora) - (inicio)).count() > (long int) this->g->getNumberNodes()/4) {
            std::cout << "\nIG Abortado por Tempo\n";
            this->solution = std::make_shared<Solution>(sBest);
            return this->solution;
        }
    }
    this->solution = std::make_shared<Solution>(sBest);

    std::cout << "\nIG Completo\n";
    return this->buscaLocal(inicio);
}

sol_ptr Algorithms::buscaLocal(tempo_t inicio) {
    Solution s = *this->solution;

    size_t no_improv = 0;
    while (true) {
        // seleciona cluster
        size_t id_c = rand() % s.clusters.size();
        std::shared_ptr<Cluster> cluster = s.clusters.at(id_c);
        
        // seleciona vértice
        std::vector<int> c_nodes = cluster->getNodes();
        size_t it = rand() % c_nodes.size();
        int id_n = *(c_nodes.begin() + it);
        node_ptr node = this->g->getNode(id_n);


        // calcula impacto no cluster atual
        if (cluster->current_bound - node->weight() >= cluster->lower_bound) {
            double impact = 0;
            for (size_t i=0; i<c_nodes.size(); ++i) {
                impact += node->edge_weight(c_nodes.at(i));
            }

            // calcula impacto nos outros clusters e insere no primeiro
            // que melhorar
            double new_impact = 0;
            for (size_t i=0; i<s.clusters.size(); ++i) {
                if ((i != id_c) && (node->weight() + s.clusters.at(i)->current_bound <= s.clusters.at(i)->upper_bound)) {
                    std::vector<int> aux_nodes = s.clusters.at(i)->getNodes();
                    for (size_t j=0; j<aux_nodes.size(); ++j) {
                        new_impact += node->edge_weight(aux_nodes.at(j));
                    }

                    if (new_impact > impact) {
                        c_nodes.erase(c_nodes.begin() + it);
                        cluster->clearCluster();
                        s.clusters.at(i)->clearCluster();
                        aux_nodes.push_back(node->id());
                        for (size_t k=0; k<c_nodes.size(); k++) {
                            cluster->insertNode(this->g->getNode(c_nodes.at(k)));
                        }

                        for (size_t k=0; k<aux_nodes.size(); k++) {
                            s.clusters.at(i)->insertNode(this->g->getNode(aux_nodes.at(k)));
                        }
                        this->insert_edges_cluster2(i);
                        this->insert_edges_cluster2(id_c);
                        no_improv = 0;
                        break;
                    }
                }
            }
            no_improv++;
        }
        tempo_t agora = relogio_t::now();
        if (no_improv > 1000 || 
        std::chrono::duration_cast<std::chrono::seconds>((agora) - (inicio)).count() > (long int) this->g->getNumberNodes()/4) {
            break;
        }
    }



    this->solution = std::make_shared<Solution>(s);
    return this->solution; 
}