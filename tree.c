//
//  tree.c
//  expr
//
//  Created by sallaben on 10/14/17.
//

#include "tree.h"

/*
 * Tree struct definition
 */
struct Tree {
    Node root;                                          //stores the root node of this tree
    Queue raw;                                          //stores a Queue representing the raw input expression (derived from the parse tree)
    Queue expression;                                   //stores a Queue respresenting the processed expression (digits and symbols properly grouped)
    Queue operands;                                     //stores a Queue representing the operands
    Stack operators;                                    //stores a Stack representing the operators
    Queue result;                                       //stores a Queue used to process the final parsed expression to an integer
};

/*
 * Allocate and initialize a new Tree instance
 */
Tree Tree_new(Node root) {
    Tree tree = malloc(sizeof(struct Tree));
    tree->root = root;
    tree->raw = Queue_new();
    tree->expression = Queue_new();
    tree->operands = Queue_new();
    tree->operators = Stack_new();
    tree->result = Queue_new();
    return tree;
}

/*
 * Parses the tree into a formatted expression
 */
void Tree_parse(Tree tree) {
    subTree_parse(tree, tree->root);
    Queue raw = tree->raw;
    Queue expression = tree->expression;
    Queue operands = tree->operands;
    int digit = 0;
    int place = 0;
    while(Queue_size(raw) > 0) {
        int c = Queue_dequeue(raw);
        int i = c - '0';                                        //converts char value to integer value (i.e. '1' to 1)
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
}

/*
 * Parses the subtree into a formatted subexpression
 */
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

/*
 * Evaluates the formatted expression using reverse Polish notation and the shunting-yard algorithm
 * Returns an integer representing the value of that expression
 */
int Tree_evaluate(Tree tree) {
    Queue expression = tree->expression;
    Queue operands = tree->operands;
    Stack operators = tree->operators;
    Queue result = tree->result;
    Stack evaluation = Stack_new();
    /*
     * Shunting-yard algorithm
     */
    while(Queue_size(expression) > 0) {                             //while there are symbols in the formatted expression queue
        char c = Queue_dequeue(expression);                             //get the next symbol
        if(c == '#') {                                                  //if it is a number
            Queue_enqueue(result, Queue_dequeue(operands));                 //enqueue its integer value (from the operands queue)
        }
        if(c == '+' || c == '-' ||
           c == '*' || c == '/') {                                      //if it is an operand
            while(T_precedence(c, Stack_peek(operators)) >= 0) {             //while it has greater or equal precedence compared to the top of the stack
                Queue_enqueue(result, Stack_pop(operators));                    //pop it from the operators stack and enqueue
            }
            Stack_push(operators, c);                                       //push the operator to the stack
        }
        if(c == '(') {                                                  //if it is a '('
            Stack_push(operators, c);                                       //push it to the stack
        }
        if(c == ')') {                                                  //if it is a ')'
            while(Stack_peek(operators) != '(') {                           //pop off the stack and enqueue until parentheses are balanced
                if(Stack_size(operators) == 0) {
                    return -1;
                }
                Queue_enqueue(result, Stack_pop(operators));
            }
            Stack_pop(operators);                                           //pop it off the stack
        }
    }
    if(Queue_size(expression) == 0) {
        while(Stack_size(operators) != 0) {
            Queue_enqueue(result, Stack_pop(operators));
        }
        /*
         * Evaluates shunting-yard algorithm
         */
        int qsize = Queue_size(result);
        for(int i = 0; i < qsize; i++) {
            int c = Queue_dequeue(result);
            if(c == '+' || c == '-' ||
               c == '*' || c == '/') {                                  //if the next char is an operator
                int op2 = Stack_pop(evaluation);                            //pop operand 2 from the stack
                int op1 = Stack_pop(evaluation);                            //pop operand 1 from the stack
                int resultant;
                switch(c) {                                                 //perform subevaluation
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
    return Stack_pop(evaluation);                                   //returns the final integer result of the evaluation
}

/*
 * Returns the precedence relation of the two given operators:
 * +1 if original is more important than compare, 0 if they are equal, -1 if original is less important than compare
 */
int T_precedence(char original, char compare) {
    if(original == '+' || original == '-') {
        if(compare == '*' || compare == '/') {
            return 1;
        } else if(compare == '+' || compare == '-') {
            return 0;
        } else {
            return -1;
        }
    } else {
        if(compare == '*' || compare == '/') {
            return 1;
        } else {
            return -1;
        }
    }
}

/*
 * Prints the Tree recursively
 */
void Tree_print(Tree tree) {
    if(tree->root != NULL) {
        printf("\n ** Printing tree... **\n\n");
        subTree_print(tree->root, 1);
    }
}

/*
 * Prints a subTree recursively
 */
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

/*
 * Returns true if the given char is a digit character, false otherwise
 */
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

/*
 * Returns the Tree's root node
 */
Node Tree_root(Tree tree) {
    return tree->root;
}

/*
 * Frees the Tree from memory
 */
void Tree_free(Tree tree) {
    Queue_free(tree->expression);
    Queue_free(tree->raw);
    Queue_free(tree->operands);
    Queue_free(tree->result);
    Stack_free(tree->operators);
    Node_free(tree->root);
    free(tree);
}
