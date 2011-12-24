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
_avl_node_exchange(avl_node_t **n1,avl_node_t **n2)
{
  avl_node_t *tmp = *n1;
  *n1 = *n2;
  *n2 = tmp;
}

static inline void 
_avl_recal_balance(int left,avl_node_t *node,avl_node_t *new)
{
  if(left){
    if(new)
      node->lbalance = (1 + MAX(new->lbalance,new->rbalance));
    else
      node->lbalance = 0;
  }
  else{
    if(new)
      node->rbalance = (1 + MAX(new->lbalance,new->rbalance));
    else
      node->rbalance = 0;
  }
}

static inline void 
_avl_rotate_left(avl_tree_t *tree,avl_node_t *n1,avl_node_t *n2)
{
  n2->right = n1->left;
  if(n1->left) { n1->left->p = n2; }
  _avl_recal_balance(0,n2,n1->left);

  n1->left = n2;
  _avl_recal_balance(1,n1,n2);


  if(n2->p){
    if(n2->p->left == n2) {
      _avl_recal_balance(1,n2->p,n1);
      n2->p->left = n1;
    }
    else if(n2->p->right == n2) {
      _avl_recal_balance(0,n2->p,n1);
      n2->p->right = n1;
    }
  }
  else{
    tree->root = n1;
  }
  n1->p = n2->p;
  n2->p = n1;
}

static inline void 
_avl_rotate_right(avl_tree_t *tree,avl_node_t *n1,avl_node_t *n2)
{
  n2->left = n1->right;
  if(n1->right) { n1->right->p = n2; }
  _avl_recal_balance(1,n2,n1->right);

  n1->right = n2;
  _avl_recal_balance(0,n1,n2);

  if(n2->p){
    if(n2->p->left == n2) {
      _avl_recal_balance(1,n2->p,n1);
      n2->p->left = n1;
    }
    else if(n2->p->right == n2) {
      _avl_recal_balance(0,n2->p,n1);
      n2->p->right = n1;
    }
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
	_avl_rotate_right(tree,n,pn);
	pn = n->p;
      }
      else if(pn->left ==n && n->right == node){
	_avl_rotate_left(tree,node,n);
	_avl_node_exchange(&node,&n);
	_avl_rotate_right(tree,n,pn);
	pn = n->p;
      }
      else if(pn->right == n && n->right == node){
	_avl_rotate_left(tree,n,pn);
	pn = n->p;
      }
      else if(pn->right == n && n->left == node){
	_avl_rotate_right(tree,node,n);
	_avl_node_exchange(&node,&n);
	_avl_rotate_left(tree,n,pn);
	pn = n->p;
      }
      //else{ printf ("ERR"); printf ("ERR"); printf ("ERR"); printf ("ERR"); }
    }
    else{
      node = node->p;
      n = pn;
      pn = pn->p;
    }
  }

  return 1;
}


static inline void
_avl_print_node(avl_node_t *node)
{
  printf("key:%d,lb:%d,rb:%d,lkey:%d,rkey:%d,pkey:%d\n",
	 node->key,
	 node->lbalance,
	 node->rbalance,
	 node->left ? node->left->key : -1,
	 node->right ? node->right->key : -1,
	 node->p ? node->p->key : -1);
}


static inline 
void _avl_dump_node(avl_node_t *node)
{
  if(node->left){
    _avl_dump_node(node->left);
  }
  
  _avl_print_node(node);

  if(node->right){
    _avl_dump_node(node->right);
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
  _avl_dump_node(node->left);
  _avl_print_node(node);
  _avl_dump_node(node->right);
}
