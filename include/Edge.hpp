#ifndef EDGE_HPP
#define EDGE_HPP

class Edge
{
    private:
        int _idNode1;
        int _idNode2;
        float _weight;
    public:
        Edge(int idN1, int idN2, float weight);
        
        int idNode1();
        int idNode2();
        float weight();
};

#endif // EDGE_HPP