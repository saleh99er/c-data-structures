#include <stdlib.h>;

#define DATA_SIZE_FAULT 3
#define MALLOC_FAULT 4
#define BAD_ARGS_DS 5
#define DS_FAILED 6 //data struct invar failed, prob failed its check

typedef int bool;
enum{false, true};

struct NiceGraph{
    bool isSym;
    bool isRef;
    bool isTrans;

    int numNodes;
    bool** AdjacMatrix;
    void* nodes;
};

struct Pair{
    int one;
    int two;
};

struct NiceGraph * initEmptyNG(int numOfNodes, bool isSymmetric, bool isReflexive, bool isTransitive){
    
    if(numOfNodes < 1){
        exit(BAD_ARGS_DS);
    }

    struct NiceGraph *ng = (struct NiceGraph*)malloc(sizeof(struct NiceGraph));
    
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
        //maybe not allocate based on number of nodes here ^^
    
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

struct Pair* initPair(int firstOne, int secondOne){

    if(firstOne < 0 || secondOne < 0)
        exit(BAD_ARGS_DS);

    struct Pair *p = (struct Pair*)malloc(sizeof(struct Pair));
    (*p).one = firstOne;
    (*p).two = secondOne;

    return p;
}

struct Pair** initPairs(int* firstArray, int* secondArray, int size){
    if(size < 1)
        exit(BAD_ARGS_DS);

    struct Pair ** pl = (struct Pair**)malloc(size * sizeof(struct Pair));
    for(int i=0;i<size;i++){
        pl[i] = initPair(firstArray[i],secondArray[i]);
    }
}

struct NiceGraph * initNG(int numOfNodes, bool isSymmetric, bool isReflexive, bool isTransitive, struct Pair** adjacList, int aListLength){
    struct NiceGraph* ng = initEmptyNG(numOfNodes, isSymmetric, isReflexive, isTransitive);
    for(int i=0;i<aListLength; i++){
        addEdge(ng, (adjacList[i])->one, (adjacList[i])->two);
    }
    return ng;
    //printf("WIP");
}

void addEdge(struct NiceGraph* ng, struct Pair* p){
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

void addEdgeNC(struct NiceGraph* ng, int one, int two){
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

void removeEdge(struct NiceGraph* ng, struct Pair* p){
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

void removeEdgeNC(struct NiceGraph* ng, int one, int two){
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

struct NiceGraph* removeNode(struct NiceGraph* ng, int node){
    int newNumNodes = ng->numNodes - 1;
    struct NiceGraph* newNG = initEmptyNG(newNumNodes, ng->isSym, ng->isRef, ng->isTrans);
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

bool isDirectConnect(struct NiceGraph* ng, int one, int two){
    return ng->AdjacMatrix[one][two];
}

//very unsure if this will work
bool isConnected(struct NiceGraph* ng, int one, int two, bool* travSet){
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

int findLeaf(struct NiceGraph* ng){
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

bool isCyclic(struct NiceGraph* ng){
    while(ng->numNodes > 0){
        int leaf = findLeaf(ng);
        if(leaf == -1) return true; //implicitly we know there are more than 0 nodes
        else ng = removeNode(ng, leaf);
    }
    return false;
}

void debugAdjMatrix(struct NiceGraph* ng){
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

void debugPL(struct Pair** pl, int size){
    printf("Printing PairList");
    for(int i = 0; i < size; i++)
        printf("[%d] -> [%d] \n", (pl[i])->one, (pl[i])->two);
}

void freeNG(struct NiceGraph* ng){
    for(int i = 0; i < ng->numNodes; i++){
        free(ng->AdjacMatrix[i]);
    }
    free(ng->AdjacMatrix);
    free(ng);
}

void freePL(struct Pair** pl, int size){
    for(int i = 0;i<size; i++){
        free(pl[i]);
    }
    free(pl);
}

//comment out when using the lib
int main(){

    //6. Creating a pair list
    int ingoing[6] = {1,1,2,2,3,3};
    int outgoing[6] = {2,3,1,3,1,2};
    printf("Arrays initialized \n");
    struct Pair ** pl1 = initPairs(ingoing,outgoing,6);
    printf("Pair List initialized \n");
    printf("first pair %d %d \n",pl1[0]->one, pl1[1]->two);
    printf("jerry rigged sample of first pair in list \n");
    debugPL(pl1, 6);
    //7. Use initNG with above pair list
    printf("debugged pair list \n");
    freePL(pl1,6);
    
    return 0;
}
