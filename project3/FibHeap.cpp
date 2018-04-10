#include<stdio.h>
#include<stdlib.h>
#include"FibHeap.h"
#include<math.h>

//initialize FibnacciHeap
FibHeap* fib_heapmake()
{
	FibHeap *heap = nullptr;
	heap = (FibHeap*)malloc(sizeof(FibHeap));
	heap->keyNum = 0;
	heap->maxNumofDegree = 0;
	heap->min = NULL;
	heap->cons = NULL;
	return heap;
}

//initialize FibnacciNode
static FibNode* fib_makenode(int vertex, int value)
{
    FibNode * node;

    node = (FibNode *) malloc(sizeof(FibNode));
    node->vertex = vertex;
    node->key    = value;
    node->degree = 0;
    node->left   = node;
    node->right  = node;
    node->parent = NULL;
    node->child  = NULL;

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

//Remove node from root list
void fib_removenode(FibNode *node){
	node->left->right = node->right;
    node->right->left = node->left;
}

//Insert the node to the FibHeap
void fib_insert(FibHeap *heap,FibNode *node)
{
	if (heap->keyNum == 0){
		heap->min = node;
	}
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
		h1->keyNum+=h2->keyNum;
		free(h2->cons);
		free(h2);
	}

	return h1;
}

void renew_degree(FibNode *parent, int degree)
{
    parent->degree = parent->degree - degree;
    if (parent-> parent == NULL) return;
    renew_degree(parent->parent, degree);
}

void fib_cut(FibHeap *heap, FibNode *node, FibNode *parent)
{
	fib_removenode(node);
	renew_degree(parent, node->degree);
	//node has no sibling
	if (node == node->right)
    	parent->child = NULL;
    else 
    	parent->child = node->right;

	node->parent = NULL;
	node->left = node->right = node;
	node->marked = 0;
	//add the subtree the node in to the root list
	fib_addnode(node, heap->min);
}

void fib_cascadingcut(FibHeap *heap, FibNode *node) 
{
	FibNode *parent = node->parent;
	if(parent != NULL) return;

	if(node->marked == 0) node->marked = 1;
    else
	{
		fib_cut(heap, node, parent);
		fib_cascadingcut(heap, parent);
	}
}

void fib_decrease(FibHeap *heap, FibNode *node, int key)
{
    if(heap==NULL || heap->min==NULL || node==NULL) return;

	if( key >= node->key) 
    {
        printf("only increasing operation is allowed\n");
        return;
    }

    node->key = key;
    FibNode* parent = node->parent;
    if (parent != NULL && node->key < parent->key)
    {
        //cut off node and add it to the root list
        fib_cut(heap, node, parent);
        fib_cascadingcut(heap, parent);
    }

    // update min node
    if (node->key < heap->min->key)
        heap->min = node;
}

void fib_makecons(FibHeap * heap)
{
    int old = heap->maxNumofDegree;

    heap->maxNumofDegree = (int)(log((double)(heap->keyNum)) / log(2.0) + 1);

    //if the old is enough
    if (old >= heap->maxNumofDegree)
        return;

    //maxDegree+1 for possible union
    heap->cons = (FibNode **)realloc(heap->cons, sizeof(FibHeap *) * (heap->maxNumofDegree + 1));
}

FibNode *fib_removemin(FibHeap *heap)
{
    FibNode *min = heap->min;

    if (heap->min == min->right){
        heap->min = NULL;    	
	}
    else{
        fib_removenode(min);
        heap->min = min->right;
    }
    min->left = min->right = min;

    return min;
}

void fib_link(FibHeap * heap, FibNode * node, FibNode *root)
{
    //remove the node
    fib_removenode(node);
    //add node as root's child
    if (root->child == NULL)
        root->child = node;
    else
        fib_addnode(node, root->child);

    node->parent = root;
    root->degree++;
    node->marked = 0;
}

void fib_consolidate(FibHeap *heap)
{
    int i, d, D;
    FibNode *x, *t, *p;

    fib_makecons(heap);	//open space for hashing
    D = heap->maxNumofDegree + 1;

    for (i = 0; i < D; i++)
        heap->cons[i] = NULL;
 
    //union root nodes with same degree
    while (heap->min != NULL)
    {
        x = fib_removemin(heap);    //remove the min tree
        d = x->degree;                    //get its degree
        // heap->cons[d] != NULL means degree collision
        while (heap->cons[d] != NULL)
        {
            t = heap->cons[d];            //t has same degree as x 
            if (x->key > t->key)        //find smaller one as root
            {
                p = t;
                x = t;
                t = p;
            }
            fib_link(heap, t, x);    //link t to x
            heap->cons[d] = NULL;
            d++;
        }
        heap->cons[d] = x;
    }
    heap->min = NULL;
 
    //re-add node in heap->cons to root list
    for (i = 0; i < D; i++)
    {
        if (heap->cons[i] != NULL)
        {
            if (heap->min == NULL)
                heap->min = heap->cons[i];
            else
            {
                fib_addnode(heap->cons[i], heap->min);
                if ((heap->cons[i])->key < heap->min->key)
                    heap->min = heap->cons[i];
            }
        }
    }
}

FibNode* fib_extractmin(FibHeap *heap)
{
    if(heap==NULL || heap->min==NULL) return NULL;

    FibNode *child;
    FibNode *min = heap->min;
    // add all children of min to the root list
    while (min->child != NULL)
    {
        child = min->child;
        fib_removenode(child);
        if (child->right == child)
            min->child = NULL;
        else
            min->child = child->right;

        fib_addnode(child, heap->min);
        child->parent = NULL;
    }

    //remove min from the list
    fib_removenode(min);
    //if min is the only node in the list
    if (min->right == min)
        heap->min = NULL;
    else
    {
        heap->min = min->right;
        fib_consolidate(heap);
    }
    heap->keyNum--;

    return min;
}

int main(){
	int i;
	FibHeap* heap = fib_heapmake();
	for(i = 100; i >20; i--){
		fib_insert(heap, fib_makenode(0,i));
	}
	fib_decrease(heap, heap->min, 2);
	fib_decrease(heap, heap->min->left, 3);
	fib_decrease(heap, heap->min->right, 4);
	printf("%d ", heap->min->key);
}
