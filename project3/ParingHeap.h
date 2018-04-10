#define insert Insert
#define update DecreaseKey
#define findMin FindMin
#define deleteMin DeleteMin
struct Node {
    int vertex;
    int key;
    Node* left;
    Node* right;
    Node* child;
};
typedef Node* Root;
Root root;
Node* insert(int vertex, int value);
void update(Node* node, int value);
Node* findMin();
Node* deleteMin();
void initHeap();
