#pragma once

#include "tree.hpp"

enum ErrorCodes {
    OKAY = 0,
    WRONG_ARGUMENTS_NUMBER,
    FILE_OPENING_ERROR,
    SIZE_GETTING_ERROR,
    MEM_ALLOCATION_ERROR,
};

#define RETURN_ON_ERROR(func) { ErrorCodes error = (func); if (error) return error; }
                   
#ifdef USE_SPEAKER
    #define outputString(...) {                                     \
        printf(__VA_ARGS__);                                        \
                                                                    \
        char dumpCmd[MAX_CMD_LEN] = {0};                            \
        strcpy(dumpCmd, "gtts en \"");                              \
        sprintf(dumpCmd + strlen(dumpCmd), __VA_ARGS__);            \
        deletePunctuation(dumpCmd);                                 \
        strcpy(dumpCmd + strlen(dumpCmd), "\" > /dev/null 2>&1");   \
                                                                    \
        system(dumpCmd);                                            \
    }
#else
    #define outputString(...) printf(__VA_ARGS__);
#endif

void playGame(node *curNode);
void addNewCharacter(node *curNode, char name[], char feature[]);
void showCharDescription(node *curNode);
bool printPositiveFeatures(nodePathElem_t path[], char name[]);
bool printNegativeFeatures(nodePathElem_t path[], char name[], bool isPosPrinted);
void showDifferences(node *curNode);
size_t printCommonFeatures(nodePathElem_t path1[], nodePathElem_t path2[]);
void printDiffrentFeatures(nodePathElem_t path[], size_t commonIdx);
void loadGameMenu(node *curNode);
ErrorCodes readDataFromPath(const char *path, char **string, size_t *szFile);
int readOneChar();
void deletePunctuation(char string[]);