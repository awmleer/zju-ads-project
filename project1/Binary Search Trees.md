# Binary Search Trees#

**author:**

**Date:2018-03-16**

---

#### Chapter 1: Introduction####

​	Analyze and compare the performances of a sequence of insertions and deletions between **unbalanced binary search trees**, **AVL trees** and **splay trees**.

---

#### Chapter 2: Data Structure / Algorithm Specification####

*Structure of a binary tree :*

```c
struct TreeNode{
    Element key;
    TreeNode *left;
    TreeNode *right;
    TreeNode *parent;
    Value Height;
}
```

*Algorithms of find :*

```c
Position Find( Element x, Tree T )
{
    if( T == NULL ) return NULL;
    if( T->key == x ) return T;
    if( T->key > x ) return Find( x, T->left );
    if( T->key < x ) return Find( x, T->right );
}
```

*Operation of unbalanced trees :*

```c++
TreeNode Insert(Element x, TreeNode T)
{
    if( T == NULL )
    {
        T = new TreeNode;
        T->key = x;
        T->left = T->right = NULL;
    }
    else if( x < T->key)
            T->left = Insert( x, T->left );
    else if( x > T->key )
                T->right = Insert( x, T->right );
    return T;
}


TreeNode Delete( Element x, TreeNode T )
{
    if( T == NULL )  
        return T;
    else if( x < T->key) 
            T->left = Delete( x, T->left );
    else if( x > T->key )
            T->right = Delete( x, T->right );
    else  
        {
            if( T->left == NULL )
            {
            	Position P = T->right;
            	free(T);
            	return P;   
            }
            else
            {
            	Position P = FindMax( T->left );
            	T->key = P->key;
            	T->left = Delete( P->key, T->left );
            }
        }
    return T;
}
```

*Operation of AVL trees :*

```c
Position SingleLeft( Position k2 )
{
    Position k1;
    k1 = k2->left;
    k2->left = k1->right;
    if(k2->left != NULL)
        k2->left->parent = k2;
    k1->right = k2;
    k1->parent = k2->parent;
    k2->parent = k1;
    
    k2->height = Max( Height( k2->left ), Height( k2->right ) ) + 1;
    k1->height = Max( Height( k1->left ), Height( k2->right ) ) + 1;
    
    return k1;
}

Position DoubleLeft( Position k3 )
{
    k3->left = SingleRight( k3->left );
    return SingleLeft( k3 );
}

static Position SingleRight( Position k2 )
{
    Position k1;
    k1 = k2->right;
    if(k1 == NULL){
        return k2;
    }
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
```

![](C:\Users\11099\Pictures\实验步骤\捕获.PNG)

```c++
TreeNode InsertAVL( Element x, TreeNode T )
{
    if( T == NULL )
    {
        T = new TreeNode
        T->key = x;
        T->height = 0;
        T->left = T->right = NULL;
    }
    else
        if( x < T->key)
        {
            T->left = InsertAVL( x, T->left );
            if( Height( T->left ) - Height( T->right ) == 2 )
            {
                if( x < T->left->key )
                    T = SingleLeft(T);
                else
                    T = DoubleLeft(T);
            }
        }
        else
            if( x > T->key )
            {
                T->right = InsertAVL( x, T->right );
                if( Height( T->right ) - Height( T->left ) == 2 )
                {
                    if( x > T->right->key )
                        T = SingleRight(T);
                    else
                        T = DoubleRight(T);
                }
            }
    T->height = Max( Height( T->left ), Height( T->right ) ) + 1;
    return T;
}


TreeNode DeleteAVL( Element x, TreeNode T )
{
    if( T == NULL )
        return T;
    else if( x < T->key)
        {
            T->left = DeleteAVL( x, T->left );
            if( Height( T->right ) - Height( T->left ) == 2 )
            {
                if( T->right->left == NULL || ( T->right->right != NULL && T->right->left->height < T->right->right->height ) ) 
                    T = SingleRight(T);
                else T = DoubleRight(T); 
            }
        }
        else if( x > T->key )
            {
                T->right = DeleteAVL( x, T->right );
                if( Height( T->left ) - Height( T->right ) == 2 )
                {
                    if( T->left->right == NULL || ( T->left->left != NULL && T->left->left->height > T->left->right->height ) ) 
                        T = SingleRight(T);
                    else
                        T = DoubleLeft(T); 
                }
            }
        else 
            {
                if( T->left == NULL ){
                    Position P = T->right;
                    free(T);
                    return P;
                }
                else if( T->right == NULL ){
                    Position P = T->left;
                    free(T);
                    return P;
                }
                else if( T->left->height > T->right->height )
                {
                    Position P = FindMax( T->left );
                    T->key = P->key;
                    T->left = DeleteAVL( P->key, T->left );
                }
                else
                {
                    Position P = FindMin( T->right );
                    T->key = P->key;
                    T->right = DeleteAVL( P->key, T->right );
                }
            }
    T->height = Max( Height( T->left ), Height( T->right ) ) + 1;
    return T;
}
```

*Operation of splay trees :*

```c++
TreeNode DeleteSPL( Element x, TreeNode T )
{
    Position P = Find( x, T );
    if(P == NULL) 
        return T;
    T = Splay( P, NULL ); 
    if(T->left == NULL){
        if(T->right != NULL)
            T->right->parent = NULL;
        P = T->right; 
        free(T);
        return P;
    }
    P = FindMax(T->left); 
    P = Splay( P, T ); 
    P->right = T->right;  
    if(P->right != NULL){
        P->right->parent = P;
    }
    P->parent = NULL;
    free(T);
    return P;
}


TreeNode InsertWithParent( Element x, TreeNode T, Position parent ) 
{
    if( T == NULL ) 
    {
        T = new TreeNode;
        T->key = x;
        T->left = T->right = NULL;
        T->parent = parent;  
    }
    else
        if( x < T->key)
        {
            T->left = InsertWithParent( x, T->left, T );
        }
        else
            if( x > T->key ) 
            {
                T->right = InsertWithParent( x, T->right, T );
            }
    return T;
}


TreeNode Splay( Position c, Position T )
{
    Position p = c->parent;   
    if(p == T || p == NULL) 
        return c;
    Position g = p->parent;  
    if(g == T || g == NULL)   
    {
        if(c->key < p->key){
            return Splay( SingleLeft(p), T );
        }
        else
        {
            return Splay( SingleRight(p), T );
        }
    }
    else
    {
        if(g->parent != NULL) 
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
```



