#include <iostream>
#include "ParingHeap.h"
#include <limits>

#define MAX std::numeric_limits<int>::max()

using namespace std;

struct Edge{
    int weight;
    struct Node* node;
    struct Edge* next;
};


int main() {
//    int x = MAX;
    int verticesCount;
    cin >> verticesCount;
    struct Node* nodes[verticesCount];
    for (int i = 0; i < verticesCount; ++i) {
        nodes[i] = insert(i, MAX);
    }
    update(nodes[0], 0);
//    TODO init edges
    while(root != nullptr){
        struct Node* n = findMin();
//        deleteMin =
    }
//    cout << x << endl;
    return 0;
}
