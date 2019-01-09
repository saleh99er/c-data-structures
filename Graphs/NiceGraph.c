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

    //debug statement
    printf("Initialized Nice Graph with %d nodes and the following prop: \n",numOfNodes);
    if(isSymmetric) //to lazy to do proper string formatting for this
        printf("Symmetric,");
    if(isReflexive)
        printf("Reflexive,");
    if(isTransitive)
        printf("Transitive");
    if(!isSymmetric && !isReflexive && !isTransitive)
        printf("N/a");
    printf("\n");

    if(isReflexive){
        //since we know the graph is reflexive, each node has a connection to itself
        for(int i = 0; i < numOfNodes; i++)
            ng->AdjacMatrix[i][i] = true;
    }

    return ng;
}

struct NiceGraph * initNG(int numOfNodes, bool isSymmetric, bool isReflexive, bool isTransitive, int* in, int* out, int eListSize){
    struct NiceGraph* ng = initEmptyNG(numOfNodes, isSymmetric, isReflexive, isTransitive);
    for(int i=0; i<eListSize ; i++) {
        printf("%d -> %d \n", in[i] , out[i]);
        addEdge(ng, in[i] , out[i] );
    }
    return ng;
    //printf("WIP");
}

void addEdge(struct NiceGraph* ng, int one, int two){
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

void removeEdge(struct NiceGraph* ng, int one, int two){
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

void freeNG(struct NiceGraph* ng){
    for(int i = 0; i < ng->numNodes; i++){
        free(ng->AdjacMatrix[i]);
    }
    free(ng->AdjacMatrix);
    free(ng);
}

void printEdges(int* in, int* out, int size){
    for(int i = 0; i < size; i++)
        printf("%d -> %d \n", in[i], out[i]);
}

//comment out when using the lib
int main(){

    //1. empty Nice Graph init
    struct NiceGraph * ng1 = initEmptyNG(7, true, true, true);
    if(!(*ng1).isSym || !(*ng1).isRef || !(*ng1).isTrans)
        //printf("DS failure");
        exit(DS_FAILED);
    printf("%d\n",(*ng1).AdjacMatrix[0][0]);

    //2 debug print the adjacency matrix
    debugAdjMatrix(ng1);

    //3. adding 1->3 edge on ng1
    addEdge(ng1, 1,3);
    debugAdjMatrix(ng1);

    //4. Creating a no sym, reflex, trans, graph with
    //empty Nice Graph init and same edge added
    struct NiceGraph* ng2 = initEmptyNG(5, false, true, true);
    printf("%d %d %d \n", ng2->isSym, ng2->isRef, ng2->isTrans);

    // if((*ng2).isSym || !(*ng2).isRef || !(*ng2).isTrans)
    //     printf("DS failure \n");
    //     exit(DS_FAILED);

    //5. adding 1->3 edge on ng2
    addEdge(ng2, 1,3);
    debugAdjMatrix(ng2);

    //6. Creating a in->out list and using initNG
    int ingoing[6] = {0,0,1,1,2,2};
    printf("Ingoing: %x\n",ingoing);
    int outgoing[6] = {1,2,0,2,0,1};
    printf("Outgoing: %x\n",outgoing);
    //printEdges(ingoing,outgoing, 6);
    struct NiceGraph* ng3 = initNG(3,false,false,false,ingoing,outgoing, 6);
    //debugAdjMatrix(ng3);
    //7.


    //passed all checks, successful

    freeNG(ng1);
    freeNG(ng2);
    freeNG(ng3);

    return 0;
}
