#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include "tree.hpp"

size_t buildTreeFromStr(char *inputSequence, size_t length, size_t i, node *curNode) {
    assert(inputSequence);

    size_t nameStartIdx = i;
    bool leftLeafIsCreated = false;
    while (i < length) {
        if (inputSequence[i] == '(' || inputSequence[i] == ')') {
            if (!curNode->data) {
                size_t nameLen = i - nameStartIdx;
                curNode->data = (char*)calloc(nameLen + 1, sizeof(char));
                strncpy(curNode->data, inputSequence + nameStartIdx, nameLen);
            }

            if (inputSequence[i] == '(') {
                if (leftLeafIsCreated) {
                    curNode->right = createNode(curNode);
                    i = buildTreeFromStr(inputSequence, length, i+1, curNode->right);
                }
                else {
                    curNode->left = createNode(curNode);
                    i = buildTreeFromStr(inputSequence, length, i+1, curNode->left);
                    leftLeafIsCreated = true;
                }
            }
            else if (inputSequence[i] == ')') {
                return i;
            }
        }
        ++i;
    }
    return i;
}

node* createNode(node *parent) {
    node *newNode = (node*) calloc(1, sizeof(node));
    newNode->parent = parent;
    newNode->data = nullptr;
    newNode->left = newNode->right = nullptr;

    return newNode;
}

void setDataValue(node *curNode, char value[]) {
    curNode->data = (char*) calloc(1, strlen(value) + 1);
    strcpy(curNode->data, value);
}

// Each node have only two or zero leafs.
void clearTree(node *curNode) {
    if (curNode->left->left) {
        clearTree(curNode->left);
    }

    if (curNode->right->right) {
        clearTree(curNode->right);
    }

    if (curNode->left->left == nullptr) {
        free(curNode->left->data);
        curNode->left->data = nullptr;
        free(curNode->left);
        curNode->left = nullptr;
    }
    if (curNode->right->left == nullptr) {
        free(curNode->right->data);
        curNode->right->data = nullptr;
        free(curNode->right);
        curNode->right = nullptr;
    }
    free(curNode->data);
    curNode->data = nullptr;
}