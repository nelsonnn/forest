#ifndef Tree
#define Tree
#include <vector>

struct Vertex { float x, y, z };

class Tree : public Renderable
{
public:
    Tree(float* Base, float* Direction, float StepSize, float MaxDepth,
         float ParentWeight, float ChildWeight, float SiblingWeight);
    Tree(float* Base, float* Direction, Tree* Parent);
    ~Tree();

    void Branch();
    void Grow();

private:
    Tree* parent; // Tree that this instance branches from
    std::vector<Tree*> siblings; // Trees that share the same parent as this tree
    std::vector<Tree*> children; // Trees that branch from this tree

    Vertex base; // [x, y, z] of where this tree starts
    Vertex direction; // [x, y, z] direction that this tree grows
    float stepSize; // How far each node is from another
    std::vector<Vertex*> nodes; // the vertices that define the tree's skeleton
    float maxDepth; // If maxDepth == 0 this tree cannot have child branches
    /*
    When calculating new growth direction,
    how much this tree grows away from its parent,
    siblings and children
    */
    float parentWeight;
    float childWeight;
    float siblingWeight;
};
