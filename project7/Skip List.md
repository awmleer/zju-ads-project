## Skip List

#### Chapter 1

​	Skip list is a data structure that supports both searching and insertion in O(log*N*) expected time. The operation is very fast by maintaining a linked hierarchy of subsequences, with each successive subsequence skipping over fewer elements than the previous one.

#### Chapter 2

**Struct:**

```C++
typedef struct NodeStruct Node;
struct NodeStruct{
    int key;
    int value;
    Node *forward[1];
};

typedef struct ListStruct List;
struct ListStruct{
    int level;
    Node *header;
};
```

**Initialize:**

A skip list is built in layers. The bottom layer is an ordinary ordered linked list. Each higher layer acts as an "express lane" for the lists below, where an element in layer ![i](https://wikimedia.org/api/rest_v1/media/math/render/svg/add78d8608ad86e54951b8c8bd6c8d8416533d20) appears in layer ![i+1](https://wikimedia.org/api/rest_v1/media/math/render/svg/2fe1bfc8314922e4c3fdb4e8eceb20a00b4f011d) with some fixed probability ![p](https://wikimedia.org/api/rest_v1/media/math/render/svg/81eac1e205430d1f40810df36a0edffdc367af36) (two commonly used values for ![p](https://wikimedia.org/api/rest_v1/media/math/render/svg/81eac1e205430d1f40810df36a0edffdc367af36) are ![{\frac {1}{2}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/a11cfb2fdb143693b1daf78fcb5c11a023cb1c55) or![\frac{1}{4}](https://wikimedia.org/api/rest_v1/media/math/render/svg/a2dfb63ee75ec084f2abb25d248bc151a2687508)). On average, each element appears in ![{\displaystyle {\frac {1}{1-p}}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/5f7924af7cc11107f8304ec42323a1ad0b034672) lists, and the tallest element (usually a special head element at the front of the skip list) in all the lists. The skip list contains ![{\displaystyle \log _{\frac {1}{p}}n\,}](https://wikimedia.org/api/rest_v1/media/math/render/svg/ed3cb9aa9a9d7b6593b5d23b7eea9bb0f1b557a7) lists. 

```C++
List* create()  
{
    List *sl=(skiplist *)malloc(sizeof(List));    
    sl->level=0;
    sl->header=createNode(MAX_LEVEL-1,0,0);    
    for(int i=0;i<MAX_LEVEL;i++)    
        sl->header->forward[i]=NULL; 
    return sl;  
}  
```

**Search:**

A search for a target element begins at the head element in the top list, and proceeds horizontally until the current element is greater than or equal to the target. If the current element is equal to the target, it has been found. If the current element is greater than the target, or the search reaches the end of the linked list, the procedure is repeated after returning to the previous element and dropping down vertically to the next lower list. The expected number of steps in each linked list is at most ![{\frac {1}{p}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/31025cccb5c9d719d490bfc933e8d7b6b6f2b425), which can be seen by tracing the search path backwards from the target until reaching an element that appears in the next higher list or reaching the beginning of the current list. Therefore, the total *expected* cost of a search is ![{\displaystyle {\frac {\log _{\frac {1}{p}}n}{p}}}](https://wikimedia.org/api/rest_v1/media/math/render/svg/67074ae981cbf5fef1362caab705e9e0209a25a3) which is ![{\mathcal {O}}(\log n)\,](https://wikimedia.org/api/rest_v1/media/math/render/svg/0d4564f8652da4d6bc379228c67a2e1f86214ae8), when ![p](https://wikimedia.org/api/rest_v1/media/math/render/svg/81eac1e205430d1f40810df36a0edffdc367af36) is a constant. By choosing different values of ![p](https://wikimedia.org/api/rest_v1/media/math/render/svg/81eac1e205430d1f40810df36a0edffdc367af36), it is possible to trade search costs against storage costs. 

```c++
int search(skiplist *sl,int key)  
{  
    nodeStructure *p,*q=NULL;  
    p=sl->header;  
    int k=sl->level;  
    for(int i=k-1; i >= 0; i--){  
        while((q=p->forward[i])&&(q->key<=key)){  
            if(q->key==key)  
                return q->value;  
            p=q;
    	}
    }
    return 0;  
}  
```

**Insert:**

```c++
bool insert(List *sl,int key,int value)  
{  
    Node *update[MAX_LEVEL];  
    Node *p, *q = NULL;  
    p=sl->header;  
    int k=sl->level;  
    for(int i=k-1; i >= 0; i--){  
        while((q=p->forward[i])&&(q->key<key))  
            p=q;   
        update[i]=p;  
    }  
    if(q&&q->key==key)  
        return false;  
    k=randomLevel();  
    if(k>(sl->level))  
    {  
        for(int i=sl->level; i < k; i++){  
            update[i] = sl->header;  
        }  
        sl->level=k;  
    }  
    q=createNode(k,key,value);  
    for(int i=0;i<k;i++)  
    {  
        q->forward[i]=update[i]->forward[i];  
        update[i]->forward[i]=q;  
    }  
    return true;  
}  
```

**Delete:**

```c++
bool deleteSL(List *sl,int key)  
{  
    Node *update[MAX_LEVEL];    
    Node *p,*q=NULL; 
    p=sl->header; 
    int k=sl->level;  
    for(int i=k-1; i >= 0; i--){
        while((q=p->forward[i])&&(q->key<key))  
            p=q;  
        update[i]=p;  
    }  
    if(q&&q->key==key)  
    { 
        for(int i=0; i<sl->level; i++){    
            if(update[i]->forward[i]==q){    
                update[i]->forward[i]=q->forward[i];    
            }  
        }   
        free(q);  
        for(int i=sl->level-1; i >= 0; i--){    
            if(sl->header->forward[i]==NULL){    
                sl->level--;    
            }    
        }   
        return true;  
    }  
    else  
        return false;  
}  
```

