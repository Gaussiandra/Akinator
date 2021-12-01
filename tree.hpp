const size_t MAX_NODE_COUNT = 250;

struct node {
    char *data   = nullptr;
    node *left   = nullptr, 
         *right  = nullptr, 
         *parent = nullptr;
};

struct nodePathElem_t {
    bool isLeftChild = true;
    node *nodePtr = nullptr;
};

size_t buildTreeFromStr(char *inputSequence, size_t length, size_t i, node *root);
bool getNodePath(nodePathElem_t path[], const char name[], node *curNode, bool isLeft, size_t arrSize);
node* createNode(node *parent = nullptr);
void setDataValue(node *curNode, char value[]);
void clearTree(node *curNode);