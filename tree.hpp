struct node {
    char *data   = nullptr;
    node *left   = nullptr, 
         *right  = nullptr, 
         *parent = nullptr;
};

size_t buildTreeFromStr(char *inputSequence, size_t length, size_t i, node *root);
node* createNode(node *parent = nullptr);
void setDataValue(node *curNode, char value[]);
void clearTree(node *curNode);