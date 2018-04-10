#include <iostream>
//#include "ParingHeap.h"
#include "FibHeap.h"
#include <limits>
#include <fstream>

#define MAX std::numeric_limits<int>::max()

using namespace std;

struct Edge{
    int weight;
    Node* node;
    Edge* next;
};

void addEdge(Node* nodes[], Edge* edgeLists[], int a, int b, int weight){
    Edge* e = (Edge*)malloc(sizeof(struct Edge));
    e->weight = weight;
    e->node = nodes[b];
    e->next = nullptr;
    Edge* x = edgeLists[a];
    if(x == nullptr){
        edgeLists[a] = e;
        return;
    }
    while(x->next != nullptr){
        x=x->next;
    }
    x->next = e;
}

int main() {
//    int x = MAX;
    //open the data file
    ifstream inFile;
    inFile.open("data.gr");
    if (!inFile) {
        cerr << "Unable to open file data.gr.";
        exit(1);
    }
    //initialize graph
    int verticesCount, edgesCount;
    Node** nodes;
    Edge** edgeLists;
    initHeap();
    char c;
    while (inFile >> c){
        string line;
        if(c=='a'){
            //read edges
            int a;
            int b;
            int weight;
            inFile >> a >> b >> weight;
            cout << a << ' ' << b << endl;
            a = a - 1;
            b = b - 1;
            addEdge(nodes, edgeLists, a, b, weight);
            addEdge(nodes, edgeLists, b, a, weight);
        }else if(c=='p'){
            inFile >> c >> c;
            inFile >> verticesCount >> edgesCount;
            cout << verticesCount << endl;
            cout << edgesCount << endl;
            nodes = new Node*[verticesCount];
            edgeLists = new Edge*[verticesCount];
            for (int i = 0; i < verticesCount; ++i) {
                nodes[i] = insert(i, MAX);
                edgeLists[i] = nullptr;
            }
        }
        getline(inFile, line);
    }


    //mark 0 as start vertex
    update(nodes[0], 0);
    //mark target vertex (the destination)
    int target = verticesCount-1;

    //search for the shortest path
    while(findMin() != nullptr){
        Node* u = findMin();
        deleteMin();
        if(u->vertex == target){
            cout << u->key << endl;
            return 0;//found the root
        }
        Edge* edge = edgeLists[u->vertex];
        while(edge != nullptr){
            Node* v = edge->node;
            int d = u->key + edge->weight;
            if(v->key > d){
                update(v, d);
            }
            edge = edge->next;
        }
    }
    return 0;
}
