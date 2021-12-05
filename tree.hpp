#pragma once

extern const size_t MAX_NODE_COUNT;
extern const size_t MAX_NODE_NAME_LEN;
extern const size_t MAX_CMD_LEN;
extern const char FIRST_NODE_NAME[];
extern const char dotSavingPath[], picSavingPath[];

struct node {
    char *data   = nullptr;
    node *left   = nullptr, 
         *right  = nullptr, 
         *parent = nullptr;
};

struct nodePathElem_t {
    bool isLeftChild = true;
    node *nodePtr = nullptr;
};

size_t buildTreeFromStr(char *inputSequence, size_t length, size_t i, node *root);
bool getNodePath(nodePathElem_t path[], const char name[], node *curNode, bool isLeft, size_t arrSize);
void dumpGraph(FILE *dotFile, node *curNode, size_t depth, const char prevName[] = FIRST_NODE_NAME);
void saveGraphPic(node *curNode);
node* createNode(node *parent = nullptr);
void setDataValue(node *curNode, char value[]);
void clearTree(node *curNode);