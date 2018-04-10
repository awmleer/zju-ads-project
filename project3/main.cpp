#include <iostream>
#include "ParingHeap.h"
#include <limits>

#define MAX std::numeric_limits<int>::max()

using namespace std;

struct Edge{
    int weight;
    Node* node;
    Edge* next;
};


int main() {
//    int x = MAX;
    int verticesCount;
    cin >> verticesCount;
    Node* nodes[verticesCount];
    Edge* edgeLists[verticesCount];
    for (int i = 0; i < verticesCount; ++i) {
        nodes[i] = insert(i, MAX);
        edgeLists[i] = nullptr;
    }
    update(nodes[0], 0);
//    TODO init edges
    
    while(root != nullptr){
        Node* u = deleteMin();
        Edge* edge = edgeLists[u->vertex];
        while(edge != nullptr){
            Node* v = edge->node;
            int d = u->key + edge->weight;
            if(v->key > d){
                update(v, d);
            }
            edge = edge->next;
        }
//        deleteMin =
    }
//    cout << x << endl;
    return 0;
}
