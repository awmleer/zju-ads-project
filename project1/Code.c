#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#define Element int
struct Node;
typedef struct Node *Position;
typedef struct Node *Tree;

Tree MakeEmpty( Tree T );
Position Find( Element x, Tree T );
Position FindMin( Tree T );
Position FindMax( Tree T );
Tree Splay( Position P, Tree T );
Tree InsertAVL( Element X, Tree T);
Tree InsertSPL( Element X, Tree T);
Tree Insert( Element X, Tree T);
Tree InsertWithParent( Element x, Tree T, Position parent );
Tree DeleteAVL( Element X, Tree T);
Tree DeleteSPL( Element X, Tree T);
Tree Delete( Element X, Tree T);
Element Retrieve( Position P );
static Position SingleLeft( Position k2 );
static Position DoubleLeft( Position k2 );
static Position SingleRight( Position k2 );
static Position DoubleRight( Position k2 );
int Max( int a, int b );
void Print( Tree T );
void Swap(int array[], int x, int y);
void Shuffle(int array[], int n);

struct Node
{
    Element key;
    Tree left;
    Tree right;
    int height;
    Position parent;
};

Position Find( Element x, Tree T )
{
    if( T == NULL ) return NULL;
    if( T->key == x ) return T;
    if( T->key > x ) return Find( x, T->left );
    if( T->key < x ) return Find( x, T->right );
}

/* operation of unbalanced tree */

Tree Insert( Element x, Tree T )    //insert an element, recursively
{
    if( T == NULL )    //when meet the leaf, add a node
    {
        T = (Tree)malloc( sizeof( struct Node ) );
        T->key = x;
        T->left = T->right = NULL;
    }
    else
        if( x < T->key)    //left
        {
            T->left = Insert( x, T->left );
        }
        else
            if( x > T->key )    //right
            {
                T->right = Insert( x, T->right );
            }
    return T;
}

Tree Delete( Element x, Tree T )    //delete an element, recursively
{
    if( T == NULL )    //if the tree is empty, return null
    {
        return T;
    }
    else
        if( x < T->key)    //recurse in the left subtree
        {
            T->left = Delete( x, T->left );
        }
        else
            if( x > T->key ) //recurse in the right subtree
            {
                T->right = Delete( x, T->right );
            }
            else    //if the element is the one to delete
            {
                if( T->left == NULL ){
                    Position P = T->right;
                    free(T);
                    return P;    //let right tree take the place of it, the height of the right tree will not change
                }
                else    //will always choose the left, except it is null
                {
                    Position P = FindMax( T->left );
                    T->key = P->key;
                    Delete( P->key, T->left );
                }
            }
    return T;
}


/* operation of AVL trees */
static int
Height( Position P )    //calculate the height of a node to the botton
{
    if( P == NULL ){
        return -1;
    }
    else{
        return P->height;
    }
}

Tree InsertAVL( Element x, Tree T )    //the insert operation of AVL tree, recursively
{
    if( T == NULL )    //when meet the leaf, add a node
    {
        T = (Tree)malloc( sizeof( struct Node ) );
        T->key = x;
        T->height = 0;
        T->left = T->right = NULL;
    }
    else
        if( x < T->key)    //left
        {
            T->left = InsertAVL( x, T->left );
            if( Height( T->left ) - Height( T->right ) == 2 )
                if( x < T->left->key )    //left-left cause a single rotation
                {
                    T = SingleLeft(T);
                }
                else T = DoubleLeft(T);    //left-right cause a double rotation
        }
        else
            if( x > T->key )    //right
            {
                T->right = InsertAVL( x, T->right );
                if( Height( T->right ) - Height( T->left ) == 2 )
                    if( x > T->right->key )    //right-right cause a single rotation
                    {
                        T = SingleRight(T);
                    }
                    else T = DoubleRight(T);    //right-left cause a double rotation
            }
    T->height = Max( Height( T->left ), Height( T->right ) ) + 1;    //update the height of the root of the subtree
    return T;
}

Tree DeleteAVL( Element x, Tree T )    //the delete operation of AVL tree, recursively
{
    if( T == NULL )    //if the tree is empty, return null
    {
        return T;
    }
    else
        if( x < T->key)    //recurse in the left subtree
        {
            T->left = DeleteAVL( x, T->left );
            if( Height( T->right ) - Height( T->left ) == 2 ){
                
                if( T->right->left == NULL || T->right->right != NULL && T->right->left->height < T->right->right->height )    //left-left cause a single rotation
                {
                    T = SingleRight(T);
                }
                else T = DoubleRight(T);    //left-right cause a double rotation
            }
        }
        else
            if( x > T->key ) //recurse in the right subtree
            {
                T->right = DeleteAVL( x, T->right );
                if( Height( T->left ) - Height( T->right ) == 2 )
                    if( T->left->right == NULL || T->left->left != NULL && T->left->left->height > T->left->right->height )    //left-left cause a single rotation
                    {
                        T = SingleRight(T);
                    }
                    else T = DoubleLeft(T);    //left-right cause a double rotation
            }
            else    //if the element is the one to delete
            {
                if( T->left == NULL ){
                    Position P = T->right;
                    free(T);
                    return P;    //let right tree take the place of it, the height of the right tree will not change
                }
                else if( T->right == NULL ){
                    Position P = T->left;
                    free(T);
                    return P;        //the same as above
                }
                //than find the maximun of left or the minimun of the right, choose the higher one so that we need no rotation at all
                else if( T->left->height > T->right->height )
                {
                    Position P = FindMax( T->left );
                    T->key = P->key;
                    DeleteAVL( P->key, T->left );
                }
                else
                {
                    Position P = FindMin( T->right );
                    T->key = P->key;
                    DeleteAVL( P->key, T->right );
                }
            }
    T->height = Max( Height( T->left ), Height( T->right ) ) + 1;
    return T;
}

Position FindMax( Tree T )    //find the maximun element in the subtree
{
    if(T == NULL) return NULL;
    if(T->right == NULL){
        return T;
    }
    else{
        return FindMax( T->right );
    }
}

Position FindMin( Tree T )    //find the minimun element in the subtree
{
    if(T == NULL) return NULL;
    if(T->left == NULL) return T;
    else return FindMin( T->left );
}

int Max( int a, int b ){
    if(a > b) return a;
    return b;
}

static Position SingleLeft( Position k2 )
{
    Position k1;
    k1 = k2->left;
    k2->left = k1->right;
    if(k2->left != NULL)
        k2->left->parent = k2;    //change the parent, for splay operation
    k1->right = k2;
    k1->parent = k2->parent;
    k2->parent = k1;
    
    k2->height = Max( Height( k2->left ), Height( k2->right ) ) + 1;
    k1->height = Max( Height( k1->left ), Height( k2->right ) ) + 1;
    
    return k1;
}

static Position DoubleLeft( Position k3 )    //double rotation is made of two single rotation
{
    k3->left = SingleRight( k3->left );
    return SingleLeft( k3 );
}

static Position SingleRight( Position k2 )
{
    Position k1;
    k1 = k2->right;
    k2->right = k1->left;
    if(k2->right != NULL)
        k2->right->parent = k2;
    k1->left = k2;
    k1->parent = k2->parent;
    k2->parent = k1;
    
    k2->height = Max( Height( k2->right ), Height( k2->left ) ) + 1;
    k1->height = Max( Height( k1->right ), Height( k2->left ) ) + 1;
    
    return k1;
}

static Position DoubleRight( Position k3 )
{
    k3->right = SingleLeft( k3->right );
    return SingleRight( k3 );
}

/* operation of splay trees */

Tree InsertSPL( Element x, Tree T )
{
    T = InsertWithParent( x, T, NULL );
    Position P = Find( x, T );
    T = Splay( P, NULL );    //after inserted, the node will be splayed to the root
    return T;
}

Tree DeleteSPL( Element x, Tree T )
{
    Position P = Find( x, T );
    if(P == NULL) return T;
    T = Splay( P, NULL );    //splay the target node
    if(T->left == NULL){
        if(T->right != NULL)
            T->right->parent = NULL;
        P = T->right;    //let the right subtree take the place of it
        free(T);
        return P;
    }
    P = FindMax(T->left);    //the max node in the left must have no right child
    P = Splay( P, T );    //splay it to the root's left child
    P->right = T->right;    //let the left subtree take the place of it
    if(P->right != NULL){
        P->right->parent = P;
    }
    P->parent = NULL;
    free(T);
    return P;
}

Tree InsertWithParent( Element x, Tree T, Position parent )    //insert an element to a splay tree, where the 'parent' in the structure is used
{
    if( T == NULL )    //when meet the leaf, add a node
    {
        T = (Tree)malloc( sizeof( struct Node ) );
        T->key = x;
        T->left = T->right = NULL;
        T->parent = parent;        //add an operation
    }
    else
        if( x < T->key)    //left
        {
            T->left = InsertWithParent( x, T->left, T );
        }
        else
            if( x > T->key )    //right
            {
                T->right = InsertWithParent( x, T->right, T );
            }
    return T;
}


Tree Splay( Position c, Position T )    //c denote child, the node we want to splay
{
    Position p = c->parent;    //p denotes parent
    if(p == T || p == NULL) return c;
    Position g = p->parent;    //g denotes grandparent
    if(g == T || g == NULL)    //do single rotation
    {
        if(c->key < p->key){
            return Splay( SingleLeft(p), T );
        }
        else
        {
            return Splay( SingleRight(p), T );
        }
    }
    else    //do a reversed double rotation
    {
        if(g->parent != NULL)    //link the grandparent's parent to the child before the rotation is done
        {
            if(g->parent->left == g){
                g->parent->left = c;
            }
            else g->parent->right = c;
        }
        if(c->key < p->key && p->key < g->key){
            SingleLeft(g);
            return Splay( SingleLeft(p), T );
        }
        else if(c->key > p->key && p->key > g->key){
            SingleRight(g);
            return Splay( SingleRight(p), T );
        }
        else if(c->key > p->key && p->key < g->key){
            return Splay( DoubleLeft(g), T );
        }
        else if(c->key < p->key && p->key > g->key){
            return Splay( DoubleRight(g), T );
        }
    }
}

/* print */
void Print( Tree T )    //for test, you can turn it to an pre/in/post-order traversal
{
    if(T == NULL) return;
    printf("%d ", T->key);
    Print( T->left );
    Print( T->right );
}

void Swap(int array[], int x, int y){
    int temp = array[x];
    array[x] = array[y];
    array[y] = temp;
}

void Shuffle(int array[], int n){
    int x;
    int y;
    int t;
    for(t=0; t<n*n; t++){
        x = rand()%n;
        y = rand()%n;
        // printf("[%d]",t);
        Swap(array,x,y);
    }
}

int main(){
    int n,type,order;
    printf("Please input the size:\n");
    scanf( "%d", &n );
    printf("Please input \"1~3\" denoting \"unbalanced tree\", \"AVL tree\", \"Splay tree\",respectively:\n");
    scanf( "%d", &type);
    printf("Please input \"1~3\" denoting:\n  1: Insert N integers in increasing order and delete them in the same order;\n  2: Insert N integers in increasing order and delete them in the reverse order;\n  3: Insert N integers in random order and delete them in random order.\n");
    scanf("%d", &order);
    
    int i;
    Tree T = NULL;
    
    int insertArray[n];
    int deleteArray[n];
    int x;
    for(x=0; x<n; x++){
        // printf("%d\n", x);
        insertArray[x] = x;
        deleteArray[x] = x;
    }
    if(order==2){
        //revert the delete array
        for(x=0; x<n/2; x++){
            Swap(deleteArray,x,n-x-1);
        }
    }
    if(order==3){
        //shuffle the insert array and delete array
        srand(time(NULL));
        Shuffle(insertArray,n);
        Shuffle(deleteArray,n);
    }
    
    
    clock_t start, end;
    start = clock();    //start timing
    
    if(type == 1){
        for(i = 0; i < n; i++){
            T = Insert( insertArray[i], T );
        }
        for(i = 0; i < n; i++){
            T = Delete( deleteArray[i], T );
        }
    }
    else if(type == 2){
        for(i = 0; i < n; i++){
            T = InsertAVL( insertArray[i], T );
        }
        for(i = 0; i < n; i++){
            T = DeleteAVL( deleteArray[i], T );
        }
    }
    else if(type == 3){
        for(i = 0; i < n; i++){
            T = InsertSPL( insertArray[i], T );
        }
        for(i = 0; i < n; i++){
            T = DeleteSPL( deleteArray[i], T );
        }
    }
    else printf("Illegal input!\n");
    
    end = clock();    //end timing
    printf("Finished!\n");
    printf("Time = %f\n",(double)(end-start)/CLOCKS_PER_SEC);//for macOS
    // printf("Time = %f\n",(double)(end-start)/CLK_TCK);//for other operating systems
}


