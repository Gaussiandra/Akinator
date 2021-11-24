#include <cstdio>
#include <cstdlib>
#include <cassert>
#include "akinator.hpp"

int main(const int argc, const char *argv[]) {
    if (argc != 2) {
        printf("Wrong number of arguments.\n");
        return WRONG_ARGUMENTS_NUMBER;
    }

    char *inputSequence = nullptr;
    size_t length = 0;
    RETURN_ON_ERROR(readDataFromPath(argv[1], &inputSequence, &length));
    node root;
    buildTreeFromStr(inputSequence, length, 1, &root);

    playGame(root);

    clearTree(&root);
    free(inputSequence);
    inputSequence = nullptr;
    return 0;
}

void playGame(node curNode) {
    while (true) {
        printf("%s%s", curNode.data, "? [Y/N] ");
        char answer = getc(stdin);
        getc(stdin);

        if (answer == 'Y') {
            if (curNode.left == nullptr || curNode.right == nullptr) {
                printf("I'm won!\n");
                return;
            }
            else {
                curNode = *curNode.left;
            }
        }
        else if (answer == 'N') {
            if (curNode.left == nullptr || curNode.right == nullptr) {
                printf("I'm lose!\n");
                return;
            }
            else {
                curNode = *curNode.right;
            }
        }
        else {
            printf("Unknown answer.\n");
        }
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