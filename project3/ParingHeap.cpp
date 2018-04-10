#include<cstdio>
#include<cstdlib>
#include"ParingHeap.h"

struct Node {
    int vertex;
    int key;
    PNode left;
    PNode right;
    PNode child;
};

void initHeap(){
    root = NULL;
}

PNode LinkPair(PNode a,PNode b){
    if(b == NULL) return a;
    if(a == NULL) return b;
    //b is linked to a as a child
    if(a->key > b->key){
        PNode al = a->left;
        b->left = al;
        if(al != NULL){
            if(al->child == a){
                //al is a's parent
                al->child = b;
            }else{
                //al is a's left sibling
                al->right = b;
            }
        }
        PNode temp;
        temp = a;
        a = b;
        b = temp;
    }
    else{
        PNode br = b->right;
        a->right = br;
        if(br != NULL) br->left = a;
    }
    //retain the sibling relation  

    PNode ac = a->child;

    //update a's child
    a->child = b;

    //update b's left and sibling pointer
    b->left = a;
    b->right = ac;

    //update a.child's pointer
    if(ac != NULL) ac->left = b;

    return a;
}

PNode MakePNode(int v, int x){	//make an new node
    PNode node = (PNode)malloc(sizeof(struct Node));
    node->key = x;
    node->vertex = v;
    node->left = NULL;
    node->right = NULL;
    node->child = NULL;
    return node;
}

PNode insert(int vertex, int value){
    PNode node = MakePNode(vertex, value);	//new node
    if(root == NULL) root = node;
    else root = LinkPair(node, root);	//paring
    return node;
}

Root MergeHeaps(PNode first){
    //the 1st run: merge pairs from the left side
    PNode second = first->right;
    PNode last = first;		//3 pointers to roll forward

    //first roll
    //roll for loop
    while(first != NULL){
        last = LinkPair(first, second);
        first = last->right;
        if(first != NULL)
            second = first->right;
        else
            second = NULL;
        if(second == NULL) break;
    }

    //the 2nd run: merge pairs from the right side
    if(first != NULL) last = first;

    PNode next = last->left;
    while(next != NULL){
        last = LinkPair(next, last);
        next = last->left;
    }
    return last;
}

PNode findMin(){
    return root;
}

void deleteMin(){
    PNode z = root;
    if(z == NULL) return;
    if(z->child == NULL)
        root = NULL;	//empty the heap
    else{
        PNode zc = z->child;
        zc->left = NULL;
        root = MergeHeaps(zc);	//merge all children of the root
    }
}

void update(PNode x, int k){	//this is only for decreasing operation
    if(x->key < k){
        printf("Increase is not allowed!\n");
        return;
    }
    x->key = k;
    if(x != root){
        //cut x subtree from its siblings
        PNode xl = x->left;
        //if x is not root, its left (i.e. xl) can never be null
        if(xl->child==x){//xl is x's parent
            xl->child = x->right;
        }else{
            //xl is x's left sibling
            xl->right = x->right;
        }
        if(x->right != NULL){
            x->right->left = xl;
        }

        //merge this tree with x subtree
        x->left = NULL;
        x->right = NULL;
        root = LinkPair(x, root);
    }
}

int main(void){	//for test
    int i;
    for(i = -10; i < 10; i++){
        insert(0, i * i);
    }
    DecreaseKey(root->child, 0);
    for(i = 0; i < 20; i++){
        printf("%d ", root->key);
        deleteMin();
    }

    printf("\n");
    printf("finished\n");
}

