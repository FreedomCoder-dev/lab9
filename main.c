#include <stdio.h>
#include "graph_adapter.h"
#include "graph_reader.h"

void printEdgeList(struct TGEdgeList *edges);

void printAdjMatrix(struct TGAdjacencyMatrix *mtx);

int case1()
{
    struct TGEdgeList* edges, *e2;

    FILE *in = fopen("../in.txt", "r");
    edges = readTGEdgeList(in);
    fclose(in);
    printEdgeList(edges);
    struct TGAdjacencyMatrix* mtx = constructTGAMatrix(edges), *m2;
    printf("\n");
    printAdjMatrix(mtx);
    e2 = constructTGEdgeList(mtx);
    printf("\n");
    printEdgeList(e2);
    m2 = constructTGAMatrix(e2);
    printf("\n");
    printAdjMatrix(m2);
}

int case2()
{
    printf("\n=====CASE 2======\n");
    struct TGEdgeList* edges, *e2;

    FILE *in = fopen("../inmtx.txt", "r");
    struct TGAdjacencyMatrix* mtx = readTGAMatrix(in), *m2;
    fclose(in);
    printf("\n");
    printAdjMatrix(mtx);
    e2 = constructTGEdgeList(mtx);
    printf("\n");
    printEdgeList(e2);
    m2 = constructTGAMatrix(e2);
    printf("\n");
    printAdjMatrix(m2);
}

int main() {
    case1();
    case2();
    return 0;
}

void printAdjMatrix(struct TGAdjacencyMatrix *mtx) {
    printf("%d\n", mtx->node_count);
    int* mtx_end = mtx->mtx_data+mtx->node_count*mtx->node_count;
    for (int* i = mtx->mtx_data; i < mtx_end; ++i) {
        if ((i != mtx->mtx_data) && (i - mtx->mtx_data) % mtx->node_count == 0)
            printf("\n");
        printf("%d ", *i);
    }
    printf("\n");
}

void printEdgeList(struct TGEdgeList *edges) {
    struct TGEdge* curr = edges->edges;
    printf("%d\n", edges->edge_count);
    for (int i = 0; i < edges->edge_count; ++i) {
        printf("%d %d %d \n", curr->node1, curr->node2, curr->weight);
        curr++;
    }
}
