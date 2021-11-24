#include "tree.hpp"

enum ErrorCodes {
    OKAY = 0,
    WRONG_ARGUMENTS_NUMBER,
    FILE_OPENING_ERROR,
    SIZE_GETTING_ERROR,
    MEM_ALLOCATION_ERROR,
};

#define RETURN_ON_ERROR(func) { ErrorCodes error = (func); if (error) return error; }

ErrorCodes readDataFromPath(const char *path, char **string, size_t *szFile);
void playGame(node curNode);