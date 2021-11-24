#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cassert>
#include "tree.hpp"

size_t buildTreeFromStr(char *inputSequence, size_t length, size_t i, node *root) {
    assert(inputSequence);

    size_t nameStartIdx = i;
    bool leftLeafIsCreated = false;
    while (i < length) {
        if (inputSequence[i] == '(' || inputSequence[i] == ')') {
            if (!root->data) {
                size_t nameLen = i - nameStartIdx;
                root->data = (char*)calloc(nameLen + 1, sizeof(char));
                strncpy(root->data, inputSequence + nameStartIdx, nameLen);
                root->data[nameLen] = '\0';
            }

            if (inputSequence[i] == '(') {
                node *newNode = (node*)calloc(1, sizeof(node));
                newNode->data = nullptr;
                newNode->left = newNode->right = nullptr;

                if (leftLeafIsCreated) {
                    root->right = newNode;
                    i = buildTreeFromStr(inputSequence, length, i+1, root->right);
                }
                else {
                    root->left = newNode;
                    i = buildTreeFromStr(inputSequence, length, i+1, root->left);
                    leftLeafIsCreated = true;
                }
            }
            else if (inputSequence[i] == ')') {
                printf("%zu\n", i);
                return i;
            }
        }
        ++i;
    }
    return i;
}

void clearTree(node *curNode) {
    if (curNode->left) {
        clearTree(curNode->left);
    }

    if (curNode->left) {
        clearTree(curNode->left);
    }

    free(curNode->left);
    curNode->left = nullptr;
    free(curNode->right);
    curNode->right = nullptr;
}