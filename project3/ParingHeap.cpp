#include<cstdio>
#include<cstdlib>
#include"ParingHeap.h"

//struct Node ;

Root root;

void initHeap(){
    root = nullptr;
}

Node* linkPair(Node *a, Node *b){
    if(b == nullptr) return a;
    if(a == nullptr) return b;
    //b is linked to a as a child
    if(a->key > b->key){
        Node* al = a->left;
        b->left = al;
        if(al != nullptr){
            if(al->child == a){
                //al is a's parent
                al->child = b;
            }else{
                //al is a's left sibling
                al->right = b;
            }
        }
        Node* temp;
        temp = a;
        a = b;
        b = temp;
    }
    else{
        Node* br = b->right;
        a->right = br;
        if(br != nullptr) br->left = a;
    }
    //retain the sibling relation  

    Node* ac = a->child;

    //update a's child
    a->child = b;

    //update b's left and sibling pointer
    b->left = a;
    b->right = ac;

    //update a.child's pointer
    if(ac != nullptr) ac->left = b;

    return a;
}

Node* makePNode(int v, int x){	//make an new node
    Node* node = (Node*)malloc(sizeof(struct Node));
    node->key = x;
    node->vertex = v;
    node->left = nullptr;
    node->right = nullptr;
    node->child = nullptr;
    return node;
}

Node* insert(int vertex, int value){
    Node* node = makePNode(vertex, value);	//new node
    if(root == nullptr) root = node;
    else root = linkPair(node, root);	//paring
    return node;
}

Root mergeHeaps(Node *first){
    //the 1st run: merge pairs from the left side
    Node* second = first->right;
    Node* last = first;		//3 pointers to roll forward

    //first roll
    //roll for loop
    while(first != nullptr){
        last = linkPair(first, second);
        first = last->right;
        if(first != nullptr)
            second = first->right;
        else
            second = nullptr;
        if(second == nullptr) break;
    }

    //the 2nd run: merge pairs from the right side
    if(first != nullptr) last = first;

    Node* next = last->left;
    while(next != nullptr){
        last = linkPair(next, last);
        next = last->left;
    }
    return last;
}

Node* findMin(){
    return root;
}

Node* deleteMin(){
    Node* z = root;
    if(z == nullptr) return z;
    if(z->child == nullptr)
        root = nullptr;	//empty the heap
    else{
        Node* zc = z->child;
        zc->left = nullptr;
        root = mergeHeaps(zc);	//merge all children of the root
    }
    return z;
}

void update(Node* x, int k){	//this is only for decreasing operation
    if(x->key < k){
        printf("Increase is not allowed!\n");
        return;
    }
    x->key = k;
    if(x != root){
        //cut x subtree from its siblings
        Node* xl = x->left;
        //if x is not root, its left (i.e. xl) can never be nullptr
        if(xl->child==x){//xl is x's parent
            xl->child = x->right;
        }else{
            //xl is x's left sibling
            xl->right = x->right;
        }
        if(x->right != nullptr){
            x->right->left = xl;
        }

        //merge this tree with x subtree
        x->left = nullptr;
        x->right = nullptr;
        root = linkPair(x, root);
    }
}

//int main(void){	//for test
//    int i;
//    for(i = -10; i < 10; i++){
//        insert(0, i * i);
//    }
//    update(root->child, 0);
//    for(i = 0; i < 20; i++){
//        printf("%d ", root->key);
//        deleteMin();
//    }
//
//    printf("\n");
//    printf("finished\n");
//}

