#include "tree.hpp"

const char dotSavingPath[] = "/home/ilya/code/akinator/dump.dot",
           picSavingPath[] = "/home/ilya/code/akinator/dump.png";

const size_t MAX_CMD_LEN = 100;
const size_t MAX_NODE_NAME_LEN = 100;
const char FIRST_NODE_NAME[] = "Z";

enum ErrorCodes {
    OKAY = 0,
    WRONG_ARGUMENTS_NUMBER,
    FILE_OPENING_ERROR,
    SIZE_GETTING_ERROR,
    MEM_ALLOCATION_ERROR,
};

#define RETURN_ON_ERROR(func) { ErrorCodes error = (func); if (error) return error; }

ErrorCodes readDataFromPath(const char *path, char **string, size_t *szFile);
void playGame(node *curNode);
void addNewCharacter(node *curNode, char name[], char feature[]);
void dumpGraph(FILE *dotFile, node *curNode, size_t depth, const char prevName[] = FIRST_NODE_NAME);
void saveGraphPic(node *curNode);
void loadGameMenu(node *curNode);