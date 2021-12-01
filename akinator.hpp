#include "tree.hpp"

const char dotSavingPath[] = "dump.dot",
           picSavingPath[] = "dump.png";

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

void playGame(node *curNode);
void addNewCharacter(node *curNode, char name[], char feature[]);
void dumpGraph(FILE *dotFile, node *curNode, size_t depth, const char prevName[] = FIRST_NODE_NAME);
void saveGraphPic(node *curNode);
void showCharDescription(node *curNode);
bool printPositiveFeatures(nodePathElem_t path[], char name[]);
bool printNegativeFeatures(nodePathElem_t path[], char name[], bool isPosPrinted);
void loadGameMenu(node *curNode);
ErrorCodes readDataFromPath(const char *path, char **string, size_t *szFile);
int readOneChar();