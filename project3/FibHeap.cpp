#include<stdio.h>
#include<stdlib.h>
#include<memory.h>

struct FibonacciHeapNode
{
	int key;
	int degree;
	FibonacciHeapNode *left;
	FibonacciHeapNode *right;
	FibonacciHeapNode *parent;
	FibonacciHeapNode *child;
	bool marked;
};
typedef struct FibonacciHeapNode FibNode;

struct FibonacciHeap
{
	int keyNum;
	FibonacciHeapNode *min;
	int maxNumofDegree;
	FibonacciHeapNode **cons;	
};
typedef struct FibonacciHeap FibHeap;

//initialize FibnacciHeap
FibHeap* fib_heapmake()
{
	FibHeap *heap = nullptr;
	heap = (FibHeap*)malloc(sizeof(FibHeap));
	memset(heap, 0, sizeof(FibHeap));
	return heap;
}

//initialize FibnacciNode
FibNode *fib_nodemake()
{
	FibNode *node = nullptr;
	node = (FibNode*)malloc(sizeof(FibNode));
	memset(node, 0, sizeof(FibNode));
	node->left = node = node->right;
	return node;
}

//Insert the node ahead of the root
void fib_addnode(FibNode *node, FibNode *root)
{
	node->left = root->left;
	root->left->right = node;
	root->left = node;
	node->right = root;
}

//Insert the node to the FibHeap
void fib_insert(FibHeap *heap, FibNode *node)
{
	if (heap->keyNum == 0)
		heap->min = node;
	else
	{
		fib_addnode(node, heap->min);
		if ((node->key < heap->min->key))
			heap->min = node;
	}
	heap->keyNum++;
}

//
void fib_union(FibNode *a,FibNode *b)
{
	FibNode *tmp = a->right;
	a->right = b->right;
	b->right->left = a;
	b->right = tmp;
	tmp->right = b;
}

FibHeap* fib_merge(FibHeap *h1,FibHeap *h2)