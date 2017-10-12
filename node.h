//
//  node.h
//  expr
//
//  Created by sallaben on 10/4/17.
//

#ifndef node_h
#define node_h

#include <stdlib.h>

typedef struct Node *Node;

extern Node Node_new(Node parent);

extern void Node_set_parent(Node node, Node parent);

extern void Node_free(Node node);

#endif /* node_h */
