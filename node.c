//
//  node.c
//  expr
//
//  Created by sallaben on 10/4/17.
//

#include "node.h"

struct Node {
    Node parent;
    Node* children;
    int num_children;
    char* content;
};

Node Node_new(Node parent, char* content) {
    Node node = malloc(sizeof(struct Node));
    node->parent = parent;
    node->children = malloc(64 * sizeof(struct Node));
    node->num_children = 0;
    node->content = content;
    return node;
}

void Node_add_child(Node node, Node child) {
    if(node->num_children < 64) {
        *(node->children + node->num_children) = child;
        node->num_children++;
    } else {
        printf("Maximum number of children [64] reached for node with data %s!", node->content);
    }
}

char* Node_get_content(Node node) {
    return node->content;
}

int Node_get_num_children(Node node) {
    return node->num_children;
}

Node Node_get_child(Node node, int index) {
    if(node != NULL) {
         return *(node->children + index);
    }
    return NULL;
}

char* Node_get_child_content(Node node, int index) {
    Node ref = Node_get_child(node, index);
    return ref->content;
}

void Node_set_parent(Node node, Node parent) {
    node->parent = parent;
}

void Node_free(Node node) {
    if(node->parent != NULL) {
        Node_free(node->parent);
    }
    free(node);
}
