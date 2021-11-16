//
// Created by Freedom Coder on 19.10.2021.
//

#ifndef LAB6_BTREE_H
#define LAB6_BTREE_H

typedef void* TBINode;

struct BTNode
{
    TBINode data;
    struct BTNode* parent;
    struct BTNode* left;
    struct BTNode* right;
};

struct BTree {
    struct BTNode* root;
    int (*comparator)(TBINode a, TBINode b);
};

#define BT_STRATEGY_INFIXED 0
#define BT_STRATEGY_PREFIXED 1
#define BT_STRATEGY_POSTFIXED 2
#define BT_STRATEGY_INVERSE 3

void BT_construct(struct BTree* tree, int (*comparator)(TBINode a, TBINode b));
void BT_insert(struct BTree* tree, TBINode element);
void BT_remove(struct BTree* tree, TBINode element);
int BT_contains(struct BTree* tree, TBINode element);
void BT_consume(struct BTree* tree, void (*consumer)(TBINode element), int bt_strategy);
struct BTNode* BT_next(struct BTNode* curr);
void BT_destruct(struct BTree* tree);

#endif //LAB6_BTREE_H
