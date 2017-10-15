//
//  tree.c
//  expr
//
//  Created by sallaben on 10/14/17.
//

#include "tree.h"

struct Tree {
    Node root;
    Queue raw;
    Queue expression;
    Queue operands;
    Stack operators;
    Queue result;
};

Tree Tree_new(Node root) {
    Tree tree = malloc(sizeof(struct Tree));
    tree->root = root;
    tree->raw = Queue_new(false);
    tree->expression = Queue_new(false);
    tree->operands = Queue_new(true);
    tree->operators = Stack_new();
    tree->result = Queue_new(false);
    return tree;
}

void Tree_parse(Tree tree) {
    subTree_parse(tree, tree->root);
    Queue raw = tree->raw;
    Queue expression = tree->expression;
    Queue operands = tree->operands;
    int digit = 0;
    int place = 0;
    while(Queue_size(raw) > 0) {
        int c = Queue_dequeue(raw);
        int i = c - '0';                                        //retrieves integer value of char -- thanks StackOverflow!
        if(T_digit(c)) {                                        //https://stackoverflow.com/questions/5029840/convert-char-to-int-in-c-and-c
            if(place == 0) {
                digit = i;
            } else if(place > 0) {
                digit = (10 * digit) + i;
            }
            place++;
        } else {
            if(place > 0) {
                Queue_enqueue(expression, '#');
                Queue_enqueue(operands, digit);
                place = 0;
            }
            if(c == '(' || c == ')' ||
               c == '+' || c == '-' ||
               c == '*' || c == '/' ) {
                Queue_enqueue(expression, c);
            }
        }
        if(place > 0 && Queue_size(raw) == 0) {
            Queue_enqueue(expression, '#');
            Queue_enqueue(operands, digit);
        }
    }
    free(raw);
}

void subTree_parse(Tree tree, Node node) {
    Queue raw = tree->raw;
    for(int i = 0; i < Node_get_num_children(node); i++) {
        Node child = Node_get_child(node, i);
        char* content = Node_get_content(child);
        if(strcmp(content, "(") == 0) {
            Queue_enqueue(raw, '(');
        } else if(strcmp(content, ")") == 0) {
            Queue_enqueue(raw, ')');
        } else if(strcmp(content, "+") == 0) {
            Queue_enqueue(raw, '+');
        } else if(strcmp(content, "-") == 0) {
            Queue_enqueue(raw, '-');
        } else if(strcmp(content, "*") == 0) {
            Queue_enqueue(raw, '*');
        } else if(strcmp(content, "/") == 0) {
            Queue_enqueue(raw, '/');
        } else if(T_digit(content[0])) {
            Queue_enqueue(raw, content[0]);
        }
        subTree_parse(tree, child);
    }
}

int Tree_evaluate(Tree tree) {
    Queue expression = tree->expression;
    Queue operands = tree->operands;
    Stack operators = tree->operators;
    Queue result = tree->result;
    Stack evaluation = Stack_new();
    while(Queue_size(expression) > 0) {
        char c = Queue_dequeue(expression);
        if(c == '#') {
            Queue_enqueue(result, Queue_dequeue(operands));
        }
        if(c == '+' || c == '-' ||
           c == '*' || c == '/') {
            while(T_precedence(c, Stack_peek(operators)) > 0) {
                Queue_enqueue(result, Stack_pop(operators));
            }
            Stack_push(operators, c);
        }
        if(c == '(') {
            Stack_push(operators, c);
        }
        if(c == ')') {
            while(Stack_peek(operators) != '(') {
                if(Stack_size(operators) == 0) {
                    return -1;
                }
                Queue_enqueue(result, Stack_pop(operators));
            }
            Stack_pop(operators);
        }
    }
    if(Queue_size(expression) == 0) {
        while(Stack_size(operators) != 0) {
            Queue_enqueue(result, Stack_pop(operators));
        }
        int qsize = Queue_size(result);
        for(int i = 0; i < qsize; i++) {
            int c = Queue_dequeue(result);
            if(c == '+' || c == '-' ||
               c == '*' || c == '/') {
                int op2 = Stack_pop(evaluation);
                int op1 = Stack_pop(evaluation);
                int resultant;
                switch(c) {
                    case '+': resultant = op1 + op2; break;
                    case '-': resultant = op1 - op2; break;
                    case '*': resultant = op1 * op2; break;
                    case '/': resultant = op1 / op2; break;
                    default: resultant = 0;
                }
                Stack_push(evaluation, resultant);
            } else {
                Stack_push(evaluation, c);
            }
        }
    }
    return Stack_pop(evaluation);
}

int T_precedence(char original, char compare) {
    if(original == '+' || original == '-') {
        if(compare == '*' || compare == '/' ||
           compare == '+' || compare == '-') {
            return 1;
        } else {
            return -1;
        }
    } else {                                                //original == '*' || original == '/'
        if(compare == '*' || compare == '/') {
            return 1;
        } else {
            return -1;
        }
    }
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

bool T_digit(char c) {
    if(c == '0' || c == '1' ||
       c == '2' || c == '3' ||
       c == '4' || c == '5' ||
       c == '6' || c == '7' ||
       c == '8' || c == '9') {
        return true;
    }
    return false;
}

Node Tree_root(Tree tree) {
    return tree->root;
}

void Tree_free(Tree tree) {
    free(tree->expression);
    free(tree);
}
