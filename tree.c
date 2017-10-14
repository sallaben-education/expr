//
//  tree.c
//  expr
//
//  Created by sallaben on 10/14/17.
//

#include "tree.h"

struct Tree {
    Node root;
};

Tree Tree_new(Node root) {
    Tree tree = malloc(sizeof(struct Tree));
    tree->root = root;
    return tree;
}

Node Tree_root(Tree tree) {
    return tree->root;
}

void Tree_print(Tree tree) {
    if(tree->root != NULL) {
        printf("\n ** Printing tree... **\n\n");
        subTree_print(tree->root, 1);
    }
}

void subTree_print(Node root, int indent) {
    printf(" %s\n", Node_get_content(root));
    int num_children = Node_get_num_children(root);
    for(int i = 0; i < num_children; i++) {
        for(int j = 0; j < indent; j++) {
            printf(" |");
        }
        subTree_print(Node_get_child(root, i), indent + 1);
    }
}

void Tree_free(Tree tree) {
    free(tree);
}
