#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include "tree.hpp"

const size_t MAX_NODE_COUNT = 250;

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

bool getNodePath(nodePathElem_t path[], const char name[], node *curNode, bool isLeft, size_t arrSize) {
    nodePathElem_t newElem;
    newElem.isLeftChild = isLeft;
    newElem.nodePtr = curNode;
    path[arrSize++]= newElem;

    if (!strcmp(curNode->data, name)) {
        return true;
    }
    else if ((curNode->left  && getNodePath(path, name, curNode->left,  true,  arrSize)) ||
             (curNode->right && getNodePath(path, name, curNode->right, false, arrSize))) {
        return true;
    }

    newElem.nodePtr = nullptr;
    path[--arrSize] = newElem;
    return false;
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