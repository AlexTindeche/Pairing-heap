#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

ifstream fin("data.in");
ofstream fout("data.out");

class Node
{
    int value;
    Node* lChild, * sibling;
public:
    // Constructors
    Node ()
    {
        value = -2;
        lChild = nullptr;
        sibling = nullptr;
    }

    Node(int value, Node *lChild, Node *sibling)
    {
        this -> value = value;
        this -> lChild = lChild;
        this -> sibling = sibling;
    }

    // Getters
    Node* getLChild () const
    { return lChild; }

    Node* getSibling () const
    { return sibling; }

    int getValue () const
    { return value; }

    // Setters
    void setLChild (Node* l)
    { lChild = l; }

    void setSibling (Node* s)
    { sibling = s; }

    void setValud (int val)
    { value = val; }

    /*
     * Adding a new child to the current node:
     *  -> if it doesn't have a left child then we make the new node its left child
     *  -> otherwise we make it its new child and the rest its brothers
     */
    void addChild (Node* n)
    {
        if (lChild == nullptr)
            lChild = n;
        else
        {
            n->setSibling(lChild);
            setLChild(n);
        }
    }

    // Return the root of the heap
    int root()
    { return value; }

    // And of course, the destructor
    ~Node()
    {
        delete lChild;
        delete sibling;
    }
};

// Test if the heap is empty (to see if the heap is empty just give this function the root of the heap)
bool isEmpty (Node* root)
{ return (root == nullptr); }


/*
 * Merge (for mergeheap problem we will do it as a max-heap)
 *  -> so we have two roots (linked with their childs by lChild and siblings pointer)
 *  -> we compare the roots node of the heaps
 *  -> if the root node of the first heap is smaller than the root node of the second heap then root node of the
 *     second heap becomes a left child of the root node of the first heap otherwise vice-versa
 *  -> complexity: O(1)
 */

Node *merge(Node *a, Node *b)
{
    if(isEmpty(a)) return b;
    if(isEmpty(b)) return a;

    if(a->getValue() > b -> getValue())
    { a ->addChild(b); return a; }

    else
    { b -> addChild(a); return b;}
}

// Inserting a node is just creating that node as a heap and then merging it
Node *insert(Node * a, int key)
{
    Node *b = new Node(key, nullptr, nullptr);
    return(merge(a, b));
}


// Delete the links between all nodes on the root's left child "line" by mearging all the nodes from left to right and then
// merging them back from right to left (basically from left to right it merges every two nodes and then back from right to left
// back with the heap)
Node *delete_root(Node* node)
{
    if(node == nullptr || node -> getSibling() == nullptr)
        return node;
    else
    {
        // Create 3 new heaps and merge them toghether
        Node *a = node;
        Node *b = node -> getSibling();
        Node *c = node -> getSibling() -> getSibling();

        a -> setSibling(nullptr);
        b ->setSibling(nullptr);

        return merge(merge(a, b), delete_root(c));
    }
}

vector <Node*> sets (300000);

int main ()
{
    int n, q;
    fin >> n >> q;

    // Now we know how many sets we have so we can resize our vector

    //sets.resize(n);

    // Actually this doesn't work idk why I just put the max value given

    for(int i = 0; i < q; i++)
    {
        int choice;
        fin >> choice;
        int x, y;
        if(choice == 1)
        {
            fin >> x >> y;
            sets[x] = insert(sets[x], y);
        }
        if(choice == 2)
        {
            fin >> x;
            fout << sets[x] -> root() << '\n';
            sets[x] = delete_root(sets[x] -> getLChild());
        }
        if(choice == 3)
        {
            fin >> x >> y;
            sets[x] = merge(sets[x], sets[y]);
            sets[y] = nullptr;
        }
    }
    return 0;
}