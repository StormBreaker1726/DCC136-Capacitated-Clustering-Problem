#ifndef NODE_HPP
#define NODE_HPP
class Node
{
    private:
        int _id;
        float _weight;
    public:
        Node(int id, float weight);
        
        int id();
        float weight();
};

#endif // NODE_HPP