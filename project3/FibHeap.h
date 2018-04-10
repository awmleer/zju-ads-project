#ifndef FIB_HEAP_H_
#define FIB_HEAP_H_

#define FibonacciHeapNode Node
typedef struct FibonacciHeapNode Node;
struct Node
{
    int vertex;
    int key;
    int degree;
    Node* left;
    Node* right;
    Node* parent;
    Node* child;
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
//Node* findMin();
Node* deleteMin();
FibHeap* initHeap();

#endif