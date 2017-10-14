//
//  node.h
//  expr
//
//  Created by sallaben on 10/4/17.
//

#ifndef node_h
#define node_h

#include <stdio.h>
#include <stdlib.h>

typedef struct Node *Node;

extern Node Node_new(Node parent, char* data);

extern void Node_add_child(Node node, Node child);

extern char* Node_get_content(Node node);

extern int Node_get_num_children(Node node);

extern Node Node_get_child(Node node, int index);

extern char* Node_get_child_content(Node node, int index);

extern void Node_set_parent(Node node, Node parent);

extern void Node_free(Node node);

#endif /* node_h */
