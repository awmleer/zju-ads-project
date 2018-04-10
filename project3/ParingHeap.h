#define insert Insert
#define update DecreaseKey
#define findMin FindMin
#define deleteMin DeleteMin
typedef struct Node* PNode;
typedef struct Node* Root;
struct Node;
Root root;
Node* insert(int vertex, int value);
void update(Node* node, int value);
Node* findMin();
void deleteMin();
void initHeap();
