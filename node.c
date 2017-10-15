//
//  node.c
//  expr
//
//  Created by sallaben on 10/4/17.
//

#include "node.h"

/*
 * Node struct definition
 */
struct Node {
    Node parent;
    Node* children;
    int num_children;
    char* content;
};

/*
 * Allocate and initialize a new Node instance
 */
Node Node_new(Node parent, char* content) {
    Node node = malloc(sizeof(struct Node));
    node->parent = parent;
    node->children = malloc(64 * sizeof(struct Node));
    node->num_children = 0;
    node->content = content;
    return node;
}

/*
 * Adds a new child Node to this node
 */
void Node_add_child(Node node, Node child) {
    if(node->num_children < 64) {
        *(node->children + node->num_children) = child;
        node->num_children++;
    } else {
        printf("Maximum number of children [64] reached for node with data %s!", node->content);
    }
}

/*
 * Returns the cstring content of this node
 */
char* Node_get_content(Node node) {
    return node->content;
}

/*
 * Returns the number of children this node currently has
 */
int Node_get_num_children(Node node) {
    return node->num_children;
}

/*
 * Returns the child node at int index
 */
Node Node_get_child(Node node, int index) {
    if(node != NULL) {
         return *(node->children + index);
    }
    return NULL;
}

/*
 * Returns the content of the child node at int index
 */
char* Node_get_child_content(Node node, int index) {
    Node ref = Node_get_child(node, index);
    return ref->content;
}

/*
 * Sets the parent of this node to a given Node
 */
void Node_set_parent(Node node, Node parent) {
    node->parent = parent;
}

/*
 * Frees the Node and all of its children from memory
 */
void Node_free(Node node) {
    for(int i = 0; i < Node_get_num_children(node); i++) {
        Node_free(Node_get_child(node, i));
    }
    free(node);
}
