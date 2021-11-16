//
// Created by Freedom Coder on 19.10.2021.
//
#include <stdlib.h>
#include "btree.h"

void BT_construct(struct BTree *tree, int (*comparator)(TBINode a, TBINode b)) {
    tree->root = NULL;
    tree->comparator = comparator;
}

void BT_insert(struct BTree *tree, TBINode element) {
    struct BTNode **currNode = &tree->root;
    struct BTNode *prevNode = NULL;
    while (1)
        if (*currNode)
            if (tree->comparator(element, (*currNode)->data) > 0)
                prevNode = *currNode, currNode = &(*currNode)->right;
            else
                prevNode = *currNode, currNode = &(*currNode)->left;
        else {
            *currNode = malloc(sizeof(struct BTNode));
            (*currNode)->data = element;
            (*currNode)->parent = prevNode;
            (*currNode)->left = NULL;
            (*currNode)->right = NULL;
            break;
        }
}

int BT_contains_internal(struct BTree *tree, struct BTNode *branch, TBINode element) {
    if (!branch) return 0;
    if (!tree->comparator(branch->data, element)) return 1;
    if (BT_contains_internal(tree, branch->left, element)) return 1;
    if (BT_contains_internal(tree, branch->right, element)) return 1;
    return 0;
}

int BT_contains(struct BTree *tree, TBINode element) {
    return BT_contains_internal(tree, tree->root, element);
}

void BT_remove(struct BTree *tree, TBINode element) {
    struct BTNode **currNode = &tree->root;
    while (1)
        if (*currNode) {
            int comp = tree->comparator(element, (*currNode)->data);
            if (comp > 0)
                currNode = &(*currNode)->right;
            else if (comp < 0)
                currNode = &(*currNode)->left;
            else {
                struct BTNode **replacement = &(*currNode)->left;
                while ((*replacement)->right) replacement = &(*replacement)->right;
                (*replacement)->right = (*currNode)->right;
                if (*replacement != (*currNode)->left) (*replacement)->left = (*currNode)->left;
                free(*currNode);
                *currNode = *replacement;
                *replacement = NULL;
                break;
            }
        }
        else
            break; // not found
}

void BT_consume_internal(struct BTNode *branch, void (*consumer)(TBINode element), int bt_strategy) {
    if (!branch) return;
    switch (bt_strategy) {
        case BT_STRATEGY_INFIXED:
            BT_consume_internal(branch->left, consumer, bt_strategy);
            consumer(branch->data);
            BT_consume_internal(branch->right, consumer, bt_strategy);
            break;
        case BT_STRATEGY_PREFIXED:
            consumer(branch->data);
            BT_consume_internal(branch->left, consumer, bt_strategy);
            BT_consume_internal(branch->right, consumer, bt_strategy);
            break;
        case BT_STRATEGY_POSTFIXED:
            BT_consume_internal(branch->left, consumer, bt_strategy);
            BT_consume_internal(branch->right, consumer, bt_strategy);
            consumer(branch->data);
            break;
        case BT_STRATEGY_INVERSE:
            consumer(branch->data);
            BT_consume_internal(branch->right, consumer, bt_strategy);
            BT_consume_internal(branch->left, consumer, bt_strategy);
            break;
    }
}

struct BTNode* BT_next(struct BTNode* curr) {
    if (curr)
    {
//        if (curr->left) return curr->left;
        if (curr->right) {
            curr = curr->right;
            while (curr->left) curr = curr->left;
            return curr;
        }
        return curr->parent;
    } else return NULL;
}

void BT_consume(struct BTree *tree, void (*consumer)(TBINode element), int bt_strategy) {
    BT_consume_internal(tree->root, consumer, bt_strategy);
}

void BT_destruct_internal(struct BTNode *branch) {
    if (!branch) return;
    BT_destruct_internal(branch->left);
    BT_destruct_internal(branch->right);
    free(branch);
}

void BT_destruct(struct BTree *tree) {
    BT_destruct_internal(tree->root);
}
