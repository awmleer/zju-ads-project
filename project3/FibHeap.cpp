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

//insert the node b after the node a
void fib_union(FibNode *a,FibNode *b)
{
	FibNode *tmp = a->right;
	a->right = b->right;
	b->right->left = a;
	b->right = tmp;
	tmp->left = b;
}

//merge the two FibHeap
FibHeap* fib_merge(FibHeap *h1,FibHeap *h2)
{

	//h1/h2 is nullptr
	if(h1==nullptr)
		return h2;
	if(h2==nullptr)
		return h1;
	
	//ensure the heap1 possess the max degree
	FibHeap *tmp;
	if(h1->maxNumofDegree<h2->maxNumofDegree)
	{
		tmp=h1;
		h1=h2;
		h2=tmp;
	}


	if(h1->min==nullptr)
	{
		h1->min=h2->min;
		h1->keyNum=h2->keyNum;
		free(h2->cons);
		free(h2);
	}
	else if(h2->min==nullptr)
	{
		free(h2->cons);
		free(h2);
	}
	else
	{
		fib_union(h1->min,h2->min);
		if(h1->min->key>h2->min->key)
			h1->min=h2->min;
		h1->keyNum+=k2->keyNum;
		free(h2->cons);
		free(h2);
	}

	return h1;
}

