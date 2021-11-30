#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
//#include "tree.hpp"
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

// исправить баг с вводом
void playGame(node *root) {
    node *curNode = root; ///????
    while (true) {
        printf("%s%s", curNode->data, "? [Y/N] ");
        int answer = getchar(); getchar();

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
                answer = getchar(); getchar();
                if (answer == 'Y') {
                    char charName[MAX_NODE_NAME_LEN]    = {0},
                         featureName[MAX_NODE_NAME_LEN] = {0};

                    printf("Enter new character name: ");
                    fgets(charName, MAX_NODE_NAME_LEN, stdin);
                    printf("Enter character's feature: ");
                    fgets(featureName, MAX_NODE_NAME_LEN, stdin);
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

// можно ли красивее?
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

void loadGameMenu(node *curNode) {
    while (true) {
        printf("Possible actions:\n");
        printf("1. Play akinator game.\n");
        printf("2. Show differences between objects.\n");
        printf("3. Save database visualization.\n");
        printf("4. Exit.\n");
        printf("Choose action:\n");

        int answer = getchar(); getchar();
        switch (answer) {
            case '1': playGame(curNode); break;
            case '2': break;
            case '3': saveGraphPic(curNode); break;
            case '4': return;
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