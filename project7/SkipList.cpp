#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#define MAXLEVEL 12
/* you can change the maxlevel */

/* generate random level */
int Random(int max){
	int r = rand() % max;
	return r + 1;
}

/* list node */
typedef struct NodeStruct* Node;
struct NodeStruct{
    int key;
    /* there can be values of any type, if you want */
    Node *forward;  //use array to save level-pointers
    int level;
};

/* skip list */
typedef struct ListStruct* List;
struct ListStruct{
    int level;  //the highest level
    Node head;
};

/* create a new node and make space for it */
Node MakeNode(int level) {
	Node node = (Node)malloc(sizeof(struct ListStruct));
	node->forward = (Node *)malloc(sizeof(Node) * level);
	node->level = level;
	return node;
}

/* the establishment of skip list */
List MakeList() {
	List list = (List)malloc(sizeof(struct ListStruct));
	list->level = 0;
	list->head = MakeNode(MAXLEVEL);
	list->head->key = -1;
	int i;
	for (i = 0; i < MAXLEVEL; i++) {
    	list->head->forward[i] = NULL;  //set pointers to null for each level
  	}
  	return list;
}

/* the search operation of skip list */
Node Search(int x, List list){
	Node p = list->head, next;
	int i;
	//nested loop to search the key
	for(i = list->level - 1; i >= 0; i--){
		//go ahead to reach the closest node to x in the i-th level
		for(next = p->forward[i];
		 	next && next->key < x;
		 	p = next, next = p->forward[i]); 
		//if the next step is exactly x, return
		if(next && next->key == x) return next;
	}
	return NULL;  //return not found
}

/* the insertion of skip list */
int Insert(int x, List list) {
	Node update[MAXLEVEL];//keep the nodes whose pointers are likely to be changed in the insertion
	Node p = list->head;
	Node next;
	int i;
	//nested loop to search the key, same as search()
	for(i = list->level - 1; i >= 0; i--){
		for(next = p->forward[i];
		 	next && next->key < x;
		 	p = next, next = p->forward[i]); 
		update[i] = p;
	}
	//the keys should be distinct
	if(next && next->key == x) return -1;
	
	int level = Random(list->level + 1);	//这里有待改动，是否应该以目前最高层数+1为上限？
	if(level > MAXLEVEL) level = MAXLEVEL;	//can't be higher than maxlevel
	
	//if it is higher than present level of the list, head pointers should also be updated
	if(level > list->level) {
		update[list->level] = list->head;
		list->level = level;
	}
	
	//new node
	Node node = MakeNode(level);
	node->key = x;

	//update the list of the closest nodes to x in each level, which is below x's level
	for(i = 0; i < node->level; i++){
		node->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = node;
	}
	return 0;
}

/* delete node x from skip list */ 
int Delete(int x, List list) {
	Node update[MAXLEVEL];
	Node p = list->head;
	Node next = NULL;
	int i;
	//nested loop to search the key, same as insert()
	for(i = list->level - 1; i >= 0; i--){
		for(next = p->forward[i];
		 	next && next->key < x;
		 	p = next, next = p->forward[i]); 
		update[i] = p;
	}
	//if x is not in the list, return error
	if(!next || next->key != x) return -1;
	
	//next is the exactly node to delete, update the pointers in each level, which is below x's level
	for(i = 0; i < next->level; i++)
		update[i]->forward[i] = next->forward[i];
		
	//update the highest level of the list
  	if(next->level > list->level) list->level = next->level;
	
	free(next);  //delete the node x
	
	return 0;
}

/* print the whole list, with its level-infomation */
void ShowList(List list){
	Node p = list->head;
	int i;
	printf("%d levels\n", list->level);
	//for each level, print the linked list in order
	for(i = 0; i < list->level; i++){
		p = list->head;
		printf("level %d :", i);
		while(p->forward[i])
		{
			printf("%d ", p->forward[i]->key);
			p = p->forward[i];
		}
		printf("\n");
	}

}

/* return a random number between low and up */
int RandTest(int low, int up){
	return low + rand()%(up - low);
}

int main(){
	
	srand(time(NULL)); //set rand seed
	List list = MakeList();	//empty list
	
	int i;
	int N = 100;
	/*
	//ordered sequence
	for(i = 1; i < N; i++){
		Insert(i, list);
	}
	for(i = N; i > 0; i--){
		Insert(i, list);
	}
	*/
	
	//random sequence
	int step = 100;
	for(i = 1; i < N ; i++){
		Insert(RandTest(i, i + step), list);
	}

	//search and print
	for(i = 1; i < N; i++){
		Node node = Search(i, list);
		if(node)
			printf("%d ", node->key);  //print the key founded
		else
			printf("$ ");  //if not found
	}
	printf("\n");
	
	//print list in level order
	ShowList(list);
}

//ctime included
//do not insert non-positive keys!



