#ifndef E288782C_0C6A_4F0A_B305_CE7530610665
#define E288782C_0C6A_4F0A_B305_CE7530610665

#include "Solution.hpp"
#include "Graph.hpp"

using sol_ptr = std::shared_ptr<Solution>;

class Algorithms
{
    private:
        std::shared_ptr<Graph> g;
        struct Candidate_Edge {
            /*
                Um candidato é dado por uma Aresta, que será inserida
                num cluster, dado pelo cluster_id.
            */
            int s_id;
            int t_id;
            double w_n1;
            double w_n2;
            size_t cluster_id;
            /*
                Inviável se upper_bound for atingido
            */
            bool viable;
            /*
                insertion_chance
                equivale a um valor numerico
                quanto maior, maior é a chance desse candidato entrar no cluster.
                dado por:
                A*peso da aresta + penalização
                A => valor numérico representando quanto o peso da aresta influencia na chance de inserção

                (soma dos pesos dos nós + current_bound) = contribuição do nó na bound atual do cluster
                f1 = (soma dos pesos dos nós + current_bound) / upper_bound => > 1 : acima do lower
                f2 = (soma dos pesos dos nós + current_bound) / lower_bound => > 1 : acima do upper
                penalização = B*f1 + C*f2
                B => valor numérico representando quanto o lower_bound influencia na chance de inserção
                C => valor numérico representando quanto o upper_bound influencia na chance de inserção
                Provavelmente 0 < B < 1 é melhor (maior a chance para vértices que não ultrapassam o lower_bound)
                e C >> 1 é melhor (evitar pares de nós que ultrapassem o upper_bound)
                ex:
                chance de inserção = 2 * edge.weight() + P
                P = 1/4 * f1 + 5 * f2 
            */
            double insertion_chance;
             
            Candidate_Edge(int s_id, int t_id, size_t cluster_id, double w_n1, double w_n2, double insertion_chance) {
                this->s_id = s_id;
                this->t_id = t_id;
                this->w_n1 = w_n1;
                this->w_n2 = w_n2;
                this->cluster_id = cluster_id;
                this->viable = true;
                this->insertion_chance = insertion_chance;
            };
        };

        sol_ptr solution;
        double penalization(node_ptr n1, node_ptr n2, size_t cluster_id);
        double chance_calc(edge_ptr e, size_t cluster_id);
        sol_ptr greedyHelper(float alpha, size_t iteration);
        //std::vector<Candidate_Edge> update_cand_list(std::vector<Candidate_Edge>& cand_list);
    public:
        Algorithms(std::shared_ptr<Graph> g);
        sol_ptr greedy();
        // sol_ptr localSearch();
};

#endif /* E288782C_0C6A_4F0A_B305_CE7530610665 */
