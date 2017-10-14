//
//  tree.h
//  expr
//
//  Created by sallaben on 10/14/17.
//

#ifndef tree_h
#define tree_h

#include "stdlib.h"
#include "node.h"

typedef struct Tree *Tree;

extern Tree Tree_new(Node root);

extern Node Tree_root(Tree tree);

extern void Tree_print(Tree tree);

extern void subTree_print(Node root, int indent);

extern void Tree_free(Tree tree);

#endif /* tree_h */
