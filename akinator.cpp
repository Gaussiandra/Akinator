#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
//#include "tree.hpp" ??
#include "akinator.hpp"

int main(const int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments.\n");
        return WRONG_ARGUMENTS_NUMBER;
    }

    char *inputSequence = nullptr;
    size_t length = 0;
    RETURN_ON_ERROR(readDataFromPath(argv[1], &inputSequence, &length));
    node *root = createNode();
    buildTreeFromStr(inputSequence, length, 1, root);

    loadGameMenu(root);

    clearTree(root);
    free(root);
    free(inputSequence);
    inputSequence = nullptr;
    return 0;
}

void playGame(node *curNode) {
    while (true) {
        printf("%s%s", curNode->data, "? [Y/N] ");
        int answer = readOneChar();

        if (answer == 'Y') {
            if (curNode->left == nullptr || curNode->right == nullptr) {
                printf("I won!\n");
                return;
            }
            else {
                curNode = curNode->left;
            }
        }
        else if (answer == 'N') {
            if (curNode->left == nullptr || curNode->right == nullptr) {
                printf("I lost!\n");

                printf("Do you want to add a new character? [Y/N] ");
                answer = readOneChar();
                if (answer == 'Y') {
                    char charName[MAX_NODE_NAME_LEN]    = {0},
                         featureName[MAX_NODE_NAME_LEN] = {0};

                    printf("Enter new character name: ");
                    fgets(charName, MAX_NODE_NAME_LEN, stdin);
                    charName[strlen(charName) - 1] = '\0';

                    printf("Enter character's feature: ");
                    fgets(featureName, MAX_NODE_NAME_LEN, stdin);
                    featureName[strlen(featureName) - 1] = '\0';
                    addNewCharacter(curNode, charName, featureName);
                    printf("Character was added.\n");
                } 
                else {
                    printf(":C\n");
                }
                printf("Bye!\n");

                return;
            }
            else {
                curNode = curNode->right;
            }
        }
        else {
            printf("Unknown answer.\n");
        }
    }
}

void addNewCharacter(node *curNode, char name[], char feature[]) {
    node *parent = curNode->parent;
    node **nodeToChange = nullptr;
    if (!strcmp(curNode->data, parent->right->data)) {
        nodeToChange = &parent->right;
    }
    else {
        nodeToChange = &parent->left;
    }
    *nodeToChange = createNode(parent);
    setDataValue(*nodeToChange, feature);

    (*nodeToChange)->left = createNode(*nodeToChange);
    setDataValue((*nodeToChange)->left, name);
    (*nodeToChange)->right = curNode;

    curNode->parent = *nodeToChange;
}

void dumpGraph(FILE *dotFile, node *curNode, size_t depth, const char prevName[]) {
    if (depth == 1) {
        fprintf(dotFile, "%s", "digraph G {\n");
    }

    fprintf(dotFile, "%s [shape=rect, label=\"%s\"];\n", prevName, curNode->data);

    char newName[MAX_NODE_NAME_LEN] = {0};
    strcpy(newName, prevName);
    size_t newNameLen = strlen(newName);
    if (curNode->left) {
        newName[newNameLen] = 'L';
        fprintf(dotFile, "%s -> %s;\n", prevName, newName);
        dumpGraph(dotFile, curNode->left, depth + 1, newName);
    }
    if (curNode->right) {
        newName[newNameLen] = 'R';
        fprintf(dotFile, "%s -> %s;\n", prevName, newName);
        dumpGraph(dotFile, curNode->right, depth + 1, newName);
    }

    if (depth == 1) {
        fprintf(dotFile, "%s", "}");
    }
}

void saveGraphPic(node *curNode) {
    FILE *dotFile = fopen(dotSavingPath, "w");
    dumpGraph(dotFile, curNode, 1);
    fclose(dotFile);

    char dumpCmd[MAX_CMD_LEN] = {0};
    sprintf(dumpCmd, "dot -Tpng %s -o %s", dotSavingPath, picSavingPath);
    system(dumpCmd);
}

void showCharDescription(node *curNode) {
    char name[MAX_NODE_NAME_LEN] = {0};
    printf("Enter character to describe: ");
    scanf("%s", name); getchar();

    nodePathElem_t path[MAX_NODE_COUNT];
    getNodePath(path, name, curNode, true, 0);

    if (!path[0].nodePtr) {
        printf("Character wasn't found.\n");
        return;
    }

    bool isPosPrinted = printPositiveFeatures(path, name);
    printNegativeFeatures(path, name, isPosPrinted);
    printf(".\n");
}

bool printPositiveFeatures(nodePathElem_t path[], char name[]) {
    bool isPosPrinted = false;
    for (size_t i = 0; i < MAX_NODE_COUNT - 1 && path[i + 1].nodePtr; ++i) {
        if (path[i + 1].isLeftChild && path[i+1].nodePtr) {
            if (!isPosPrinted) {
                isPosPrinted = true;
                printf("%s is ", name);
            }
            printf("%s ", path[i].nodePtr->data);
        }
    }
    
    return isPosPrinted;
}

bool printNegativeFeatures(nodePathElem_t path[], char name[], bool isPosPrinted) {
    bool isNegPrinted = false;
    for (size_t i = 0; (i < MAX_NODE_COUNT - 1) && path[i + 1].nodePtr; ++i) {
        if (!path[i + 1].isLeftChild) {
            if (isPosPrinted) {
                isPosPrinted = false;
                isNegPrinted = true;
                printf("but isn't");
            }
            else if (!isNegPrinted && !isPosPrinted) {
                isNegPrinted = true;
                printf("%s isn't", name);
            }
            printf(" %s", path[i].nodePtr->data);
        }
    }

    return isNegPrinted;
}

void showDifference(node *curNode) {
    
}

void loadGameMenu(node *curNode) {
    while (true) {
        printf("Possible actions:\n");
        printf("1. Play akinator game.\n");
        printf("2. Show character description.\n");
        printf("3. Show differences between objects.\n");
        printf("4. Save database visualization.\n");
        printf("5. Exit.\n");
        printf("Choose action:\n");

        int answer = readOneChar();
        switch (answer) {
            case '1': playGame(curNode); break;
            case '2': showCharDescription(curNode); break;
            case '3': showDifferences(curNode); break;
            case '4': saveGraphPic(curNode); break;
            case '5': return;
            default:  break;
        }

        printf("\n========\n\n");
    }
}

ErrorCodes readDataFromPath(const char *path, char **string, size_t *szFile) {
    assert(path);
    assert(string);

    FILE *file = fopen(path, "r");
    if (!file) {
        printf("File wasn't opened. Provided path: %s\n", path);
        return FILE_OPENING_ERROR;
    }
    
    fseek(file, 0, SEEK_END);
    long signedSzFile = ftell(file);
    if (signedSzFile == -1) {
        return SIZE_GETTING_ERROR;
    }
    *szFile = (size_t) signedSzFile;
    rewind(file);

    *string = (char*)calloc(*szFile + 1, sizeof(char));
    if (!*string) {
        printf("Memory wasn't allocated.\n");
        return MEM_ALLOCATION_ERROR;
    }

    // fread on Windows returns readingResult <= szFile due to deleting /r from stream
    size_t readingResult = fread(*string, sizeof(char), *szFile, file);
    *szFile = readingResult;
    *(*string + readingResult) = '\0';

    fclose(file);
    return OKAY;
}

int readOneChar() {
    int input = getchar(); getchar();
    return input;
}