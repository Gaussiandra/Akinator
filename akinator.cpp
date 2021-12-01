#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
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
    assert(curNode);

    while (true) {
        outputString("%s%s", curNode->data, "? ");
        int answer = readOneChar();

        if (answer == 'Y') {
            if (curNode->left == nullptr || curNode->right == nullptr) {
                outputString("I won!\n");
                return;
            }
            else {
                curNode = curNode->left;
            }
        }
        else if (answer == 'N') {
            if (curNode->left == nullptr || curNode->right == nullptr) {
                outputString("I lost!\n");

                outputString("Do you want to add a new character? ");
                answer = readOneChar();
                if (answer == 'Y') {
                    char charName[MAX_NODE_NAME_LEN]    = {0},
                         featureName[MAX_NODE_NAME_LEN] = {0};

                    outputString("Enter new character name: ");
                    fgets(charName, MAX_NODE_NAME_LEN, stdin);
                    charName[strlen(charName) - 1] = '\0';

                    outputString("Enter character's feature: ");
                    fgets(featureName, MAX_NODE_NAME_LEN, stdin);
                    featureName[strlen(featureName) - 1] = '\0';
                    addNewCharacter(curNode, charName, featureName);
                    outputString("Character was added.\n");
                } 
                else {
                    printf(":C\n");
                }
                outputString("Bye!\n");

                return;
            }
            else {
                curNode = curNode->right;
            }
        }
        else {
            outputString("Unknown answer.\n");
        }
    }
}

void addNewCharacter(node *curNode, char name[], char feature[]) {
    assert(curNode);
    assert(name);
    assert(feature);

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
    assert(dotFile);
    assert(curNode);
    assert(prevName);

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
    assert(curNode);

    FILE *dotFile = fopen(dotSavingPath, "w");
    dumpGraph(dotFile, curNode, 1);
    fclose(dotFile);

    char dumpCmd[MAX_CMD_LEN] = {0};
    sprintf(dumpCmd, "dot -Tpng %s -o %s", dotSavingPath, picSavingPath);
    system(dumpCmd);
}

void showCharDescription(node *curNode) {
    assert(curNode);

    char name[MAX_NODE_NAME_LEN] = {0};
    outputString("Enter character to describe: ");
    scanf("%s", name); getchar();

    nodePathElem_t path[MAX_NODE_COUNT];
    getNodePath(path, name, curNode, true, 0);

    if (!path[0].nodePtr) {
        outputString("Character wasn't found.\n");
        return;
    }

    bool isPosPrinted = printPositiveFeatures(path, name);
    printNegativeFeatures(path, name, isPosPrinted);
    printf(".\n");
}

bool printPositiveFeatures(nodePathElem_t path[], char name[]) {
    assert(path);
    assert(name);

    bool isPosPrinted = false;
    for (size_t i = 0; i < MAX_NODE_COUNT - 1 && path[i + 1].nodePtr; ++i) {
        if (path[i + 1].isLeftChild && path[i+1].nodePtr) {
            if (!isPosPrinted) {
                isPosPrinted = true;
                outputString("%s is ", name);
            }
            outputString("%s ", path[i].nodePtr->data);
        }
    }
    
    return isPosPrinted;
}

bool printNegativeFeatures(nodePathElem_t path[], char name[], bool isPosPrinted) {
    assert(path);
    assert(name);

    bool isNegPrinted = false;
    for (size_t i = 0; (i < MAX_NODE_COUNT - 1) && path[i + 1].nodePtr; ++i) {
        if (!path[i + 1].isLeftChild) {
            if (isPosPrinted) {
                isPosPrinted = false;
                isNegPrinted = true;
                outputString("but isn't");
            }
            else if (!isNegPrinted && !isPosPrinted) {
                isNegPrinted = true;
                outputString("%s isn't", name);
            }
            outputString(" %s", path[i].nodePtr->data);
        }
    }

    return isNegPrinted;
}

void showDifferences(node *curNode) {
    assert(curNode);

    char name1[MAX_NODE_NAME_LEN] = {0},
         name2[MAX_NODE_NAME_LEN] = {0};
    nodePathElem_t path1[MAX_NODE_COUNT],
                   path2[MAX_NODE_COUNT];
    outputString("Enter the first character: ");
    scanf("%s", name1); getchar(); 
    outputString("Enter the second character: ");
    scanf("%s", name2); getchar();

    getNodePath(path1, name1, curNode, true, 0);
    getNodePath(path2, name2, curNode, true, 0);

    outputString("%s and %s have in common: ", name1, name2);
    size_t commonFeatsIdx = printCommonFeatures(path1, path2);
    if (!commonFeatsIdx) {
        outputString("nothing");
    }
    outputString(".\n%s: ", name1);
    printDiffrentFeatures(path1, commonFeatsIdx);
    outputString(".\n%s: ", name2);
    printDiffrentFeatures(path2, commonFeatsIdx);
}

size_t printCommonFeatures(nodePathElem_t path1[], nodePathElem_t path2[]) {
    assert(path1);
    assert(path2);

    size_t commonFeats = 0;
    for (commonFeats = 0; commonFeats < MAX_NODE_COUNT - 2; ++commonFeats) {
        if (path1[commonFeats].nodePtr == path2[commonFeats].nodePtr &&
            path1[commonFeats+1].isLeftChild == path2[commonFeats+1].isLeftChild) {
            if (!path1[commonFeats+1].isLeftChild) {
                outputString("not ");
            }
            outputString("%s ", path1[commonFeats].nodePtr->data);
            
        }
        else {
            break;
        }

        if (!(path1[commonFeats+2].nodePtr && 
              path2[commonFeats+2].nodePtr)) {
            break;
        }
    }

    return commonFeats;
}

void printDiffrentFeatures(nodePathElem_t path[], size_t commonIdx) {
    for (size_t feats = commonIdx; (feats < MAX_NODE_COUNT - 1) && path[feats+1].nodePtr; ++feats) {
        if (!path[feats+1].isLeftChild) {
            outputString("not ");
        }
        outputString("%s ", path[feats].nodePtr->data);
    }
}

void loadGameMenu(node *curNode) {
    assert(curNode);

    while (true) {
        printf("Possible actions:\n");
        printf("1. Play akinator game (use Y/N buttons).\n");
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