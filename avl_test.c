
#include "avl.h"
#include "stdlib.h"



int
main(int len,char **args)
{
  int i = 0;
  avl_tree_t *tree = malloc(sizeof(*tree));

  if(tree){

    for(;i<5;i++){
      avl_node_t *node = malloc(sizeof(*node));
      node->key = i;
      
      avl_insert(tree,node);
    }

    avl_dump(tree);
  }

  return 1;
}
