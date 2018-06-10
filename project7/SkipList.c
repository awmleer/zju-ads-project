#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAXLEVEL 14
#define PRABABILITY 0.5 
/* you can change the maxlevel */

/* generate random level */
int Random(int max){
	int r = 1;
	//every level has probability p to ascend upwards
	while(rand() % 100 < 100 * PRABABILITY){
		r++;
		if(r >= max) return max;
	}
	return r;
}

/* list node */
typedef struct NodeStruct* Node;
struct NodeStruct{
    int key;
    int value;
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
	Node node = (Node)malloc(sizeof(struct NodeStruct));
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
	Node p = list->head, next = NULL;
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
	Node next = NULL;
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
	
	int level = Random(list->level + 1);	//random a level
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

/* this insertion will not really insert the node into the list */
int FictionInsert(int x, List list) {
	Node update[MAXLEVEL];  //keep the nodes whose pointers are likely to be changed in the insertion
	Node p = list->head;
	Node next = NULL;
	int i;
	Node fiction = MakeNode(MAXLEVEL);  //fake node
	//nested loop to search the key, same as search()
	for(i = list->level - 1; i >= 0; i--){
		for(next = p->forward[i];
		 	next && next->key < x;
		 	p = next, next = p->forward[i]);
		update[i] = fiction;
	}
	//the keys should be distinct
	if(next && next->key == x) return -1;
	
	int level = Random(list->level + 1);	//random a level
	if(level > MAXLEVEL) level = MAXLEVEL;	//can't be higher than maxlevel
	
	//if it is higher than present level of the list, head pointers should also be updated
	if(level > list->level) {
		update[list->level] = fiction;
		list->level = list->level;
	}
	
	//new node
	Node node = MakeNode(level);
	node->key = x;

	//update the list of the closest nodes to x in each level, which is below x's level
	for(i = 0; i < node->level; i++){
		node->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = node;
	}
	free(node);
	free(fiction);
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

/* delete node x from skip list */ 
int FictionDelete(int x, List list) {
	Node update[MAXLEVEL];
	Node p = list->head;
	Node next = NULL;
	int i;
	Node fiction = MakeNode(MAXLEVEL);
	//nested loop to search the key, same as insert()
	for(i = list->level - 1; i >= 0; i--){
		for(next = p->forward[i];
		 	next && next->key < x;
		 	p = next, next = p->forward[i]); 
		update[i] = fiction;
	}
	//if x is not in the list, return error
	if(!next || next->key != x) return -1;
	
	//next is the exactly node to delete, update the pointers in each level, which is below x's level
	for(i = 0; i < next->level; i++)
		update[i]->forward[i] = next->forward[i];
		
	//update the highest level of the list
  	if(next->level > list->level) list->level = list->level;
	
	free(fiction);  //delete the node x
	
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

void test(int N){
	List list = MakeList();	//empty list
	int i = 0;
	float cpu_time_used;

	printf("Size: %d ", N);

	clock_t start, end;
	unsigned long sum = 0;
	int val = 0;
	for(i=0; i<N; i++){
		val += 5;
		Insert(val, list);
	}

	start = clock();
	for(i=0; i<1000000; i++){
		val = RandTest(0, N);
		val = val*5 + 1;
		// printf("%lu\n", start);
		FictionInsert(val, list);
	}
	end = clock();
	sum = end - start;
	//print list in level order
	// ShowList(list);
	// printf("%lu\n", sum);
	cpu_time_used = ((double) sum)/CLOCKS_PER_SEC;
	printf("Insert: %lf ", cpu_time_used);

	start = clock();
	for(i=0; i<1000000; i++){
		val = RandTest(0, N);
		val = val*5;
		// printf("%lu\n", start);
		FictionDelete(val, list);
	}
	end = clock();
	sum = end - start;
	//print list in level order
	// ShowList(list);
	// printf("%lu\n", sum);
	cpu_time_used = ((double) sum)/CLOCKS_PER_SEC;
	printf("Delete: %lf ", cpu_time_used);

	start = clock();
	for(i=0; i<1000000; i++){
		val = RandTest(0, N);
		val = val*5;
		// printf("%lu\n", start);
		Search(val, list);
	}
	end = clock();
	sum = end - start;
	//print list in level order
	// ShowList(list);
	// printf("%lu\n", sum);
	cpu_time_used = ((double) sum)/CLOCKS_PER_SEC;
	printf("Search: %lf ", cpu_time_used);

	printf("\n");
}

int main(){
	srand(time(NULL)); //set rand seed

	int sizes[10] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000};
	// int sizes[19] = {5000, 10000, 15000, 20000, 25000, 30000, 35000, 40000, 45000, 50000, 55000, 60000, 65000, 70000, 75000, 80000};
	for(int i=0; i<10; i++){
		test(sizes[i]);
	}

	//search and print
	// for(i = 1; i < N; i++){
	// 	FictionDelete(335, list);
	// 	Node node = Search(i, list);
	// 	if(node)
	// 		printf("%d ", node->key);  //print the key founded
	// 	else
	// 		printf("$ ");  //if not found
	// }
	// printf("\n");
	
}
//ctime included
//do not insert non-positive keys