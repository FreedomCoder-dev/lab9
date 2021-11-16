//
// Created by Freedom Coder on 16.11.2021.
//

#include "graph_reader.h"
#include "btree.h"

struct TGAdjacencyMatrix* readTGAMatrix(FILE* in){
    int length;
    fscanf(in, "%d", &length);

    struct TGAdjacencyMatrix * mtx = malloc(sizeof(struct TGAdjacencyMatrix));
    mtx->node_count = length;
    int * curr = mtx->mtx_data = malloc(sizeof(int) * length * length);
    for (int i = 0; i < length*length; ++i)
        fscanf(in, "%d", curr++);
    return mtx;
}

#define max(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

int compareEdges(struct TGEdge* a1, struct TGEdge* b1)
{
    if (!a1) return -1;
    if (!b1) return 1;
    if(a1->node1 == b1->node1)
        if(a1->node2 == b1->node2)
            return 0;
        else
            return a1->node2 - b1->node2;
    else
        return a1->node1 - b1->node1;
}

int edgeComparator(TBINode a, TBINode b)
{
    struct TGEdge *a1 = a;
    struct TGEdge *b1 = b;
    return compareEdges(a1, b1);
}

struct TGEdgeList* readTGEdgeList(FILE* in){
    struct TGEdgeList* edgeList = malloc(sizeof(struct TGEdgeList));
    struct BTree *edges = malloc(sizeof(struct BTree));
    BT_construct(edges, edgeComparator);
    int a, b, c;
    struct TGEdge *tmpPair;
    int length;
    fscanf(in, "%d", &length);

    struct TGEdge *real_edges = malloc(sizeof(struct TGEdge) * length);
    edgeList->edge_count = length;
    edgeList->edges = real_edges;

    while ((fscanf(in, "%d %d %d", &a, &b, &c)) != EOF){
        tmpPair = malloc(sizeof(struct TGEdge));
        tmpPair->node1 = a;
        tmpPair->node2 = b;
        tmpPair->weight = c;
        BT_insert(edges, tmpPair);
    }
    /// Super ugly iterator
    int maxEdge = 0;
    struct TGEdge* prev = NULL;
    struct BTNode* curr = edges->root;
    while (curr->left) curr = curr->left;
    struct BTNode* last = edges->root;
    while (last->right) last = last->right;
    while (curr) {
        struct TGEdge * data = curr->data;
        {
            /// Iterator body
            maxEdge = max(maxEdge, data->node1);
            maxEdge = max(maxEdge, data->node2);
            if ((prev)&&((data->node2 == prev->node2 && data->node1 == prev->node1) || (data->node1 == prev->node2 && data->node2 == prev->node1)))
            {
                if (prev->weight != data->weight) {
                    printf("FAILED TO READ. FOUND DUPLICATE EDGE WITH DIFFERENT WEIGHTS. (%d %d %d), (%d %d %d)\n",
                           prev->node1, prev->node2, prev->weight, data->node1, data->node2, data->weight);
                    return edgeList;
                }
            } else {
                *real_edges++ = *data;
            }
            prev = data;
        }
        if (curr == last) break;
        curr = BT_next(curr);
    }
    BT_destruct(edges);
    edgeList->max_node = maxEdge;
    return edgeList;
}