

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
*/
struct NiceGraph * initEmptyNG(int numOfNodes, bool isSymmetric, bool isReflexive, bool isTransitive)

/*
*/
struct Pair* initPair(int firstOne, int secondOne)

/*
*/
struct Pair** initPairs(int* firstArray, int* secondArray, int size)

/*
*/
struct NiceGraph initNG(int numOfNodes, bool isSymmetric, bool isReflexive, bool isTransitive, struct Pair** adjacList)

/*
*/
void addEdge(struct NiceGraph* ng, struct Pair* p)

/*
adds Edge to NiceGraph but avoids Pair struct 
*/
void addEdgeNC(struct NiceGraph* ng, int one, int two)

/*
*/
void removeEdge(struct NiceGraph* ng, struct Pair* p)

/*
removes Edge to NiceGraph but avoids Pair struct
*/
void removeEdgeNC(struct NiceGraph* ng, int one, int two)

/*
*/
void removeNode(struct NiceGraph* ng, int node)

/*
*/
bool isDirectConnect(struct NiceGraph* ng, int one, int two)

/*
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
