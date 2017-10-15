//
//  tree.h
//  expr
//
//  Created by sallaben on 10/14/17.
//

#ifndef tree_h
#define tree_h

#include <string.h>
#include <stdlib.h>
#include "node.h"
#include "stack.h"
#include "queue.h"

typedef struct Tree *Tree;

extern Tree Tree_new(Node root);

extern void Tree_parse(Tree tree);

extern void subTree_parse(Tree tree, Node node);

extern int Tree_evaluate(Tree tree);

extern int T_precedence(char original, char compare);

extern void Tree_print(Tree tree);

extern void subTree_print(Node root, int indent);

extern bool T_digit(char input);

extern Node Tree_root(Tree tree);

extern void Tree_free(Tree tree);

#endif /* tree_h */
