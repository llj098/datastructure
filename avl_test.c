
#include "avl.h"
#include "stdlib.h"


char ch[100][4096] ;

void 
draw_node(avl_node_t *node, int height)
{
  if(!node)
    return ;


  int l = 0;
  char *c = ch[height];
  c += strlen(c);

  sprintf(c,"[k:%d,l:%d,r:%d,lb:%d,rb:%d] ", 
	  node->key,
	  node->left ? node->left->key : -1,
	  node->right ? node->right->key : -1,
	  node->lbalance,
	  node->rbalance);

  draw_node(node->left,height+1);
  draw_node(node->right,height+1);
}


void 
draw(avl_tree_t *tree)
{
  int i;

  for (i=0; i<100; ++i) {
    ch[i][0] = '\0';
  }

  draw_node(tree->root,0);
 
  for(i=0;i<10;i++){
    printf ("%s\n",ch[i]);
  }
}


int
main(int len,char **args)
{
  int i = 0;
  avl_tree_t *tree = malloc(sizeof(*tree));

  if(tree){

    for(;i<18;i++){
      avl_node_t *node = malloc(sizeof(*node));
      node->key = i;
      
      avl_insert(tree,node);
      draw(tree);
    }

    printf ("%s\n","final");
    draw(tree);
    //avl_dump(tree);
  }

  return 1;
}
