#define FibonacciHeapNode Node
typedef struct Node* PNode;
typedef struct Node* Root;
typedef struct FibonacciHeapNode FibNode;
struct Node
{
	int vertex;
	int key;
	int degree;
	FibNode* left;
	FibNode* right;
	FibNode* parent;
	FibNode* child;
	int marked;
};
struct FibonacciHeap
{
	int keyNum;
	FibonacciHeapNode *min;
	int maxNumofDegree;
	FibonacciHeapNode **cons;	
};
typedef struct FibonacciHeap FibHeap;
//FibHeap *heap;
Node* insert(int vertex, int value);
void update(Node* node, int value);
Node* findMin();
void deleteMin();
void initHeap();
