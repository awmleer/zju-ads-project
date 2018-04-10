#ifndef PARING_HEAP_H_
#define PARING_HEAP_H_

struct Node {
    int vertex;
    int key;
    Node* left;
    Node* right;
    Node* child;
};
typedef Node* Root;
extern Root root;
Node* insert(int vertex, int value);
void update(Node* node, int value);
Node* findMin();
Node* deleteMin();
void initHeap();

#endif