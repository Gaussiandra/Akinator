struct node {
    char *data = nullptr;
    node *left = nullptr, *right = nullptr;
};

size_t buildTreeFromStr(char *inputSequence, size_t length, size_t i, node *root);
void clearTree(node *curNode);