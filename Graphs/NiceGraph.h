

/*
Written by Saleh Hassen
Github: saleh99er

*/


#define DATA_SIZE_FAULT 3
#define MALLOC_FAULT 4
#define BAD_ARGS_DS 5
#define DS_FAILED 6

typedef int bool;
enum{false, true};


/*
*/
struct NiceGraph{
    bool isSym;
    bool isRef;
    bool isTrans;

    int numNodes;
    bool** AdjacMatrix;
    void* nodes;
};

/*
*/
struct Pair{
    int one;
    int two;
};

/*
initializes a NiceGraph w/ numOfNodes nodes and assigns properties (s,r,t) to
the graph. Will be a graph with no edges (unless reflexive, then x->x edges will
be present for all x between 0 and numOfNodes-1). Returns address of NiceGraph
*/
struct NiceGraph * initEmptyNG(int numOfNodes, bool isSymmetric,
    bool isReflexive, bool isTransitive)


/*
initializes a NiceGraph w/ numOfNodes nodes and assigns properties (s,r,t) to
the graph. Also puts eListSize edges, connecting all in array elements to their
corresponding out array elements. Returns address of NiceGraph.
Time Complexity: O(max(n,e)); n being numOfNodes, e being eListSize
*/
struct NiceGraph * initNG(int numOfNodes, bool isSymmetric, bool isReflexive,
    bool isTransitive, int* in, int* out, int eListSize)

/*
adds one->two Edge to NiceGraph ng
Time compListy: O(1)
*/
void addEdge(struct NiceGraph* ng, int one, int two)

/*
removes Edge one->two from NiceGraph, also removes corresponding edges depending
on graph properties
Time Complexity: O(1)
*/
void removeEdge(struct NiceGraph* ng, int one, int two)

/*
recreates NiceGraph ng, excluding that node,
Time Complexity: O(n^2) w/ n being the number of nodes in the NiceGraph
*/
struct NiceGraph* removeNode(struct NiceGraph* ng, int n)

/*
returns whether or not Node one is directly connected to Node two.
Time Complexity: O(1)
*/
bool isDirectConnect(struct NiceGraph* ng, int one, int two)

/*
returns whether or not Node one is connected in someway to Node two.
Warning: Very demanding function since it depends on removeNode multiple times
and removeNode is not implemented efficiently yet.
Time Complexity: O(n^3)
*/
bool isConnected(struct NiceGraph* ng, int one, int two, bool* travSet)

/*
helper function for isCyclic() that returns the index of a leaf node in the
NiceGraph or -1 if none is found

Worst Case: O(n^2) where n is the number of nodes in NiceGraph ng
*/
int findLeaf(struct NiceGraph* ng)

/*
https://www.cs.hmc.edu/~keller/courses/cs60/s98/examples/acyclic/
*/
bool isCyclic(struct NiceGraph* ng)

/*
*/
int numComponents(struct NiceGraph* ng)


int** compList(struct NiceGraph* ng)

/*
*/
void debugAdjMatrix(struct NiceGraph* ng){
}

/*
*/
void debugPL(struct Pair** pl)

/*
*/
void freeNG(struct NiceGraph* ng)

/*
*/
void freePL(struct Pair** pl)

/*
Running test cases, should be commented out for
header and c file when all test cases passed
and wishing to use this lib
*/
int main()
