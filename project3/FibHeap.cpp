#include<stdio.h>
#include<stdlib.h>
#include"FibHeap.h"
#include<math.h>


FibHeap *heap;

//initialize FibnacciHeap
void initHeap()
{
    heap = (FibHeap*)malloc(sizeof(FibHeap));
    heap->keyNum = 0;
    heap->maxNumofDegree = 0;
    heap->min = NULL;
    heap->cons = NULL;
}

//initialize FibnacciNode
FibNode* fib_makenode(int vertex, int value)
{
    FibNode * node = (FibNode *) malloc(sizeof(FibNode));
    //assignment
    node->vertex = vertex;
    node->key = value;
    node->degree = 0;
    node->left = node->right = node;
    node->parent = node->child = NULL;

    return node;
}

//Insert the node x ahead of the node y
void fib_addnode(FibNode *x, FibNode *y)
{
    x->left = y->left;
    y->left->right = x;
    y->left = x;
    x->right = y;
}

//Remove node x from list
void fib_removenode(FibNode *x){
    x->left->right = x->right;
    x->right->left = x->left;
}

//Insert the node to the FibHeap
Node* insert(int vertex, int value)
{
    FibNode *node = fib_makenode(vertex, value);
    if (heap->keyNum == 0){
        heap->min = node;
    }
    else{
        fib_addnode(node, heap->min);
        if(node->key < heap->min->key) heap->min = node;
    }
    heap->keyNum++;
    return node;
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

Node* findMin(){
    return heap->min;
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

//cut off a node and its subtree and add them to the root list
void fib_cut(FibHeap *heap, FibNode *node)
{
    if(node->parent == NULL) return;
    FibNode *parent = node->parent;
    fib_removenode(node);
    parent->degree--;

    if (node == node->right) parent->child = NULL;	//node with no sibling
    else parent->child = node->right;	//node with siblings

    fib_addnode(node, heap->min);	//add the subtree the node in to the root list
    node->parent = NULL;
    node->marked = 0;

    if(parent->marked == 0) parent->marked = 1;
    else fib_cut(heap, parent);		//pass on the cut operation
}

//decrease the key of a node
void update(FibNode *node, int key)
{
    if(node==NULL || heap == NULL || heap->min == NULL) return;
    //determines
    if( key >= node->key)
    {
        printf("Only decreasing operation is allowed\n");
        return;
    }

    node->key = key;	//update its key
    //cut off node and add it to the root list
    if (node->parent != NULL && node->key < node->parent->key) fib_cut(heap, node);

    // update min node
    if (node->key < heap->min->key) heap->min = node;
}

//link two nodes x and y (x as the root)
void fib_link(FibHeap * heap, FibNode * x, FibNode *y)
{
    fib_removenode(y);    //remove the node y
    //add y as x's child
    if (x->child == NULL) x->child = y;
    else fib_addnode(y, x->child);
    y->parent = x;
    y->marked = 0;

    x->degree++; //renew the degree
}

//concolidation function
void fib_consolidate(FibHeap *heap)
{
    int i, d;
    FibNode *x, *t;

    if(heap == NULL || heap->min == NULL) return;	//if the heap is empty

    heap->maxNumofDegree = (int)(log((double)(heap->keyNum)) / log(2.0) + 1);	//update the largest number of degrees
    heap->cons = (FibNode **)realloc(heap->cons, sizeof(FibHeap *) * (heap->maxNumofDegree + 1));	//open space for hashing

    for(i = 0; i < heap->maxNumofDegree + 1; i++) heap->cons[i] = NULL;

    //union root nodes with same degree
    while (heap->min != NULL)
    {
        d = heap->min->degree;                    //get its degree
        x = heap->min;
        //remove the min node
        if (heap->min == x->right){
            heap->min = NULL;
        }
        else{
            heap->min = x->right;
            fib_removenode(x);
        }
        x->left = x->right = x;	//let x be an isolated node

        FibNode *p;
        // heap->cons[d] != NULL means degree collision
        while (heap->cons[d] != NULL)
        {
            t = heap->cons[d];            //t has same degree as x
            if (x->key > t->key)        //find smaller one as root
            {
                p = x;
                x = t;
                t = p;
            }
            fib_link(heap, x, t);    //link t to x
            heap->cons[d] = NULL;	//solve the collission
            d = x->degree;	//x' new degree
        }
        heap->cons[d] = x;	//no collission
    }

    //re-add node in heap->cons to root list
    heap->min = NULL;
    for (i = 0; i < heap->maxNumofDegree + 1; i++)
    {
        if (heap->cons[i] != NULL)
        {
            if (heap->min == NULL)
            {
                heap->min = heap->cons[i];
            }
            else
            {
                fib_addnode(heap->cons[i], heap->min);
                if (heap->cons[i]->key < heap->min->key) heap->min = heap->cons[i];
            }
        }
    }
}

//delete min node and return it
void deleteMin()
{
    if(heap == NULL || heap->min == NULL) return ;

    FibNode *p;
    FibNode *min = heap->min;

    // add all children of min to the root list
    for(p = min->child; p != NULL; p = min->child)
    {
        fib_removenode(min->child);

        //if p is the only node in the list
        if (p == p->right) min->child = NULL;
        else min->child = p->right;

        //add to the root list
        fib_addnode(p, heap->min);
        p->parent = NULL;
    }

    //remove min from the list
    fib_removenode(min);

    //if min is the only node in the list
    if (min == min->right) heap->min = NULL;
    else heap->min = min->right;

    //consolidation
    fib_consolidate(heap);
    heap->keyNum--;
}

//for test
//int main(){
//    int i;
//    initHeap();
//    for(i = 1500; i >20; i--){
//        srand(i+1000);
//        insert(0,rand());
//    }
//    for(i = 150; i >20; i--){
//        deleteMin();
//    }
//    for(i = 21; i >20; i--){
//        update(heap->min->right->right->child, 3700);
//    }
//
//    printf("%d ", heap->min->key);
//}
