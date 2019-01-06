#include <stdlib.h>

#define DATA_SIZE_FAULT 3
#define MALLOC_FAULT 4
#define BAD_ARGS_DS 5
#define DS_FAILED 6

typedef int bool;
enum{false, true};

typedef struct {
    bool isSym;
    bool isRef;
    bool isTrans;

    int numNodes;
    bool** AdjacMatrix;
    void* nodes;
}NiceGraph;

typedef struct {
    int one;
    int two;
}Pair;

typedef struct {
    int size;
    Pair** list;
}PairList;

NiceGraph * initEmptyNG(int numOfNodes, bool isSymmetric, bool isReflexive, bool isTransitive){

    if(numOfNodes < 1)
        exit(BAD_ARGS_DS);

    NiceGraph *ng = (NiceGraph*)malloc(sizeof(NiceGraph));

    if(ng == NULL){
        printf("No space for empty Nice Graph");
        exit(DATA_SIZE_FAULT);
    };

    (*ng).numNodes = numOfNodes;
    (*ng).isSym = isSymmetric;
    (*ng).isRef = isReflexive;
    (*ng).isTrans = isTransitive;

    (*ng).AdjacMatrix = malloc(numOfNodes * sizeof(bool *));
    for(int i = 0;i < numOfNodes; ++i)
        (*ng).AdjacMatrix[i] = malloc(numOfNodes * sizeof(bool));

    //debug statement
    printf("Initialized Nice Graph with %d nodes and the following prop: \n",numOfNodes);
    if(isSymmetric) //to lazy to do proper string formatting for this
        printf("Symmetric,");
    if(isReflexive)
        printf("Reflexive,");
    if(isTransitive)
        printf("Transitive");
    printf("\n");

    if(isReflexive){
        //since we know the graph is reflexive, each node has a connection to itself
        for(int i = 0; i < numOfNodes; i++)
            ng->AdjacMatrix[i][i] = true;
    }

    return ng;
}

Pair* initPair(int one, int two){
    Pair* p = (Pair *)malloc(sizeof(Pair));
    p->one = one;
    p->two = two;
}

PairList * initPairs(int* firstArray, int* secondArray, int size){
    if(size < 1)
        exit(BAD_ARGS_DS);

    //NiceGraph *ng = (NiceGraph*)malloc(sizeof(NiceGraph));

    PairList* pl = (PairList*)malloc(sizeof(PairList));
    printf("PairList alloc-ed \n");
    pl->list = malloc(size * sizeof(Pair *));
    printf("List of pl alloc-ed \n");
    for(int i=0;i<pl->size;i++){
        (*pl).list[i] = initPair(firstArray[i],secondArray[i]);
    }
    printf("each element of pair pointers in list is alloc-ed \n");

    return pl;

    /*
    (*ng).AdjacMatrix = malloc(numOfNodes * sizeof(bool *));
    for(int i = 0;i < numOfNodes; ++i)
      (*ng).AdjacMatrix[i] = malloc(numOfNodes * sizeof(bool));
    */
}

NiceGraph * initNG(int numOfNodes, bool isSymmetric, bool isReflexive, bool isTransitive,  PairList* pl){
    NiceGraph* ng = initEmptyNG(numOfNodes, isSymmetric, isReflexive, isTransitive);
    for(int i=0;i<pl->size; i++){
        addEdge(ng, (pl->list[i]->one) , (pl->list[i]->two) );
    }
    return ng;
}

void addEdge(NiceGraph* ng, Pair* p){
    ng->AdjacMatrix[(p->one)][(p->two)] = true;

    //symmetric logic
    if(ng->isSym)
        ng->AdjacMatrix[(p->two)][(p->one)] = true;

    //transitive logic
    if(ng->isTrans){
        for(int i = 0; i < ng->numNodes; i++){
            if(ng->AdjacMatrix[p->two][i])
                ng->AdjacMatrix[p->one][i] = true;
        }
    }
}

void addEdgeNC(NiceGraph* ng, int one, int two){
    ng->AdjacMatrix[one][two] = true;

    //symmetric logic
    if(ng->isSym)
        ng->AdjacMatrix[two][one] = true;

    //transitive logic
    if(ng->isTrans){
        for(int i = 0; i < ng->numNodes; i++){
            if(ng->AdjacMatrix[two][i])
                ng->AdjacMatrix[one][i] = true;
        }
    }
}

void removeEdge(NiceGraph* ng,  Pair* p){
    ng->AdjacMatrix[(p->one)][(p->two)] = false;
    printf("WIP");

    //symmetric logic
    if(ng->isSym)
        ng->AdjacMatrix[(p->two)][(p->one)] = false;

    //transitive logic
    if(ng->isTrans){
        for(int i = 0; i < ng->numNodes; i++){
            if(ng->AdjacMatrix[p->two][i])
                ng->AdjacMatrix[p->one][i] = true;
        }
    }
}

void removeEdgeNC( NiceGraph* ng, int one, int two){
    ng->AdjacMatrix[one][two] = false;
    printf("WIP");

    //symmetric logic
    if(ng->isSym)
        ng->AdjacMatrix[two][one] = false;

    //transitive logic
    if(ng->isTrans){
        for(int i = 0; i < ng->numNodes; i++){
            if(ng->AdjacMatrix[two][i])
                ng->AdjacMatrix[one][i] = true;
        }
    }
}

NiceGraph* removeNode( NiceGraph* ng, int node){
    int newNumNodes = ng->numNodes - 1;
     NiceGraph* newNG = initEmptyNG(newNumNodes, ng->isSym, ng->isRef, ng->isTrans);
    int k;

    for(int i = 0; i < newNumNodes; i++){
        if(i < node) k=i;
        else if(i >= node) k = i+1;

        //copy all elements in the row but one logic
        for(int j = 0; j < newNumNodes; j++){
            if(j < node) newNG->AdjacMatrix[i][j] = ng->AdjacMatrix[k][j];
            else if(j > node) newNG->AdjacMatrix[i][j] = ng->AdjacMatrix[k][j+1];
        }
    }
    freeNG(ng);
    return newNG;
}

bool isDirectConnect( NiceGraph* ng, int one, int two){
    return ng->AdjacMatrix[one][two];
}

//very unsure if this will work
bool isConnected( NiceGraph* ng, int one, int two, bool* travSet){
    for(int neigh=0; neigh < ng->numNodes; neigh++){
        if(ng->AdjacMatrix[one][neigh] && !travSet[neigh]){
            travSet[neigh] = true;
            if(travSet[two])
                return true;
            bool isNeighConnected = isConnected(ng, neigh,two, travSet);
            if(isNeighConnected)
                return true;
        }
    }
    return false;
}

int findLeaf( NiceGraph* ng){
    int outEdges;
    for(int row = 0; row < ng->numNodes; row++){
        outEdges = 0;
        for(int col = 0; col < ng->numNodes; col++){
            outEdges += ng->AdjacMatrix[row][col];
        }
        if(outEdges == 0) return row;
    }

    return -1;
}

bool isCyclic( NiceGraph* ng){
    while(ng->numNodes > 0){
        int leaf = findLeaf(ng);
        if(leaf == -1) return true; //implicitly we know there are more than 0 nodes
        else ng = removeNode(ng, leaf);
    }
    return false;
}

void debugAdjMatrix( NiceGraph* ng){
    if(ng->numNodes > 10)
        printf("Warning that attempting to debug the adj matrix by printing is not recommended");
    printf("     ");
    for(int i = 0; i < (ng->numNodes); i++){
        printf("[%0d]", i);
    }
    printf("\n");
    for(int i = 0; i < (ng->numNodes); i++){
        printf(" [%0d] ",i);
        for(int j = 0; j < (ng->numNodes); j++){
            printf((ng->AdjacMatrix[i][j] ? " x ":" _ "));
        }
        printf("\n");
    }
}

void debugPL(PairList* pl){
    printf("Printing PairList");
    for(int i = 0; i < pl->size; i++)
        printf("[%d] -> [%d] \n", pl->list[i]->one, pl->list[i]->two);
}

void freeNG(NiceGraph* ng){
    for(int i = 0; i < ng->numNodes; i++){
        free(ng->AdjacMatrix[i]);
    }
    free(ng->AdjacMatrix);
    free(ng);
}

void freePL( PairList* pl){
    for(int i = 0;i< (pl->size); i++){
        free(pl->list[i]);
    }
    free(pl->list);
    free(pl);
}

//comment out when using the lib
int main(){

    //6. Creating a pair list
    int ingoing[6] = {1,1,2,2,3,3};
    int outgoing[6] = {2,3,1,3,1,2};
    printf("Arrays initialized \n");
     PairList* pl1 = initPairs(ingoing,outgoing,6);
    printf("Empty Pair List initialized \n");
    printf("first pair %d %d \n",pl1->list[0]->one, pl1->list[0]->two);
    printf("jerry rigged sample of first pair in list \n");
    debugPL(pl1);
    //7. Use initNG with above pair list
    //WIP

    printf("debugged pair list \n");
    freePL(pl1);
    printf("freed pair list \n");
    return 0;
}
