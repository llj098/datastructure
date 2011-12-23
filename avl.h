/*
 * a AVL tree implementation
 * Author liulijin<llj098@gmail.com>
 */

#include<stdio.h>

#define MAX(x,y) ((x) > (y) ? x : y)
#define min(x,y) ((x) > (y) ? y : x)

typedef struct avl_node_s{
  struct avl_node_s *p;
  struct avl_node_s *left;
  struct avl_node_s *right;
  int lbalance;
  int rbalance;
  int key;
  void *data;
}avl_node_t;

typedef struct avl_tree_s{
  avl_node_t *root;
  int count;
  int height;
}avl_tree_t;


static inline void 
_avl_recal_balance(int left,avl_node_t *node,avl_node_t *old,avl_node_t *new)
{
  if(left){
    if(old)
      node->lbalance -= (1 + MAX(old->lbalance,old->rbalance));
    if(new)
      node->lbalance += (1 + MAX(new->lbalance,new->rbalance));
  }
  else{
    if(old)
      node->rbalance -= (1 + MAX(old->lbalance,old->rbalance));
    if(new)
      node->rbalance += (1 + MAX(new->lbalance,new->rbalance));
  }
}

static inline void 
avl_rotate_left(avl_tree_t *tree,avl_node_t *n1,avl_node_t *n2)
{
  n2->right = n1->left;
  if(n1->left) { n1->left->p = n2; }
  _avl_recal_balance(0,n2,n1,n1->left);

  _avl_recal_balance(0,n1,n1->left,n2);
  n1->left = n2;


  if(n2->p){
    n2->p->right = n1;
    _avl_recal_balance(0,n2->p,n2->right,n1);
  }
  else{
    tree->root = n1;
  }
  n1->p = n2->p;
  n2->p = n1;
}


static inline void 
avl_rotate_right(avl_tree_t *tree,avl_node_t *n1,avl_node_t *n2)
{
  n2->left = n1->right;
  if(n1->right) { n1->right->p = n2; }
  _avl_recal_balance(1,n2,n1,n1->right);

  _avl_recal_balance(1,n1,n1->right,n2);
  n1->right = n2;

  if(n2->p){
    n2->p->left = n1;
    _avl_recal_balance(0,n2->p,n2->right,n1);
  }
  else{
    tree->root = n1;
  }
  n1->p = n2->p;
  n2->p = n1;
}


static inline 
int avl_insert(avl_tree_t *tree,avl_node_t *node)
{
  if(!tree){
    return -1;
  }

  if(!tree->root){
    tree->root = node; tree->height = 1;
    return 1;
  }

  
  int balance,left,height = 0;
  avl_node_t *pn,*n = tree->root;

  while(1){
    pn = n;
    height++;

    if(node->key > n->key){
      n = n->right;
    }
    else if(node->key < n->key){
      n = n->left;
      left = 1;
    }
    else{
      return -2;
    }

    if(!n){
      n = pn;
      pn = pn->p;
      
      if(left) {n->left = node; n->lbalance++;}
      else {n->right = node; n->rbalance++;}
      
      node->p = n;
      node->rbalance = node->lbalance = 0;
      break;
    }
  }

  /*fix balance*/
  while(pn){
    
    if(pn->left == n) pn->lbalance++;
    else if(pn->right == n) pn->rbalance++;

    balance = pn->lbalance - pn->rbalance;

    if(balance >= 2 || balance <= -2){ /*rotate*/

      if(pn->left == n && n->left == node){
	avl_rotate_right(tree,n,pn);
      }
      else if(pn->left ==n && n->right == node){
	avl_rotate_left(tree,node,n);
	avl_rotate_right(tree,node,pn);
      }
      else if(pn->right == n && n->right == node){
	avl_rotate_left(tree,n,pn);
      }
      else if(pn->right == n && n->left == node){
	avl_rotate_right(tree,node,n);
	avl_rotate_left(tree,node,pn);
      }
    }

    node = node->p;
    n = pn;
    pn = pn->p;
  }

  return 1;
}



static inline 
void _avl_dump_left(avl_node_t *node)
{
  
  if(node->left){
    _avl_dump_left(node->left);
  }
  else{
    printf("key:%d,lbalance:%d,rbalance:%d,data:%d",
	   node->key,
	   node->lbalance,
	   node->rbalance,
	   (int)node->data);
  }
 
}


static inline 
void _avl_dump_right(avl_node_t *node)
{
  if(node->right){
    _avl_dump_right(node->right);
  }
  else{
    printf("key:%d,lbalance:%d,rbalance:%d,data:%d",
	   node->key,
	   node->lbalance,
	   node->rbalance,
	   (int)node->data);
  }
 
}


static inline 
void avl_dump(avl_tree_t *tree)
{
  avl_node_t *node;
  
  if(!tree){
    printf("Tree is NULL\n");
  }

  node = tree->root;
  _avl_dump_left(node->left);
  printf("key:%d,lbalance:%d,rbalance:%d,data:%d",
	 node->key,
	 node->lbalance,
	 node->rbalance,
	 (int)node->data);
  _avl_dump_right(node->right);
}
