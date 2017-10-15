//
//  tdp.c
//  expr
//
//  Created by sallaben on 10/8/17.
//

#include "tdp.h"

int nonterminals = 6;
int terminals = 8;
enum { START, EXPRESSION, EPRIME, TERM, TPRIME, FACTOR,                      //enumerate the nonterminals 0-5
    LPAREN, RPAREN, PLUS, MINUS, MULTIPLY, DIVIDE, NUMBER, BOTTOM };         //enumerate the terminals 6-13

/*
 * TDP struct definition
 */
struct TDP {
    int index;                                              //character index measuring how much of the input has been evaluated
    const char* input;                                      //stores the full input given to the TDP as a constant
    Stack stack;                                            //the stack component of the table driven parser (making this a pushdown automaton)
    int** table;                                            //a parse table defining the grammar rules for this TDP
    Tree tree;                                              //stores the parse tree object for this TDP
};

/*
 * Allocate and initialize a new TDP instance
 */
TDP TDP_new(char* input) {
    TDP tdp = malloc(sizeof(struct TDP));                   //allocate memory for new TDP
    tdp->index = -1;                                        //set its initial index to -1, signifying that the TDP has not yet processed any input
    tdp->input = input;                                     //stores the full input expression
    tdp->stack = Stack_new();                               //initialize a new, empty Stack
    tdp->table = G_table();                                 //stores the parse table
    return tdp;
}

/*
 * Processes an input using the given TDP and table-driven parsing
 */
bool TDP_process(TDP tdp) {
    tdp->tree = Tree_new(Node_new(NULL, "START"));                      //initialize the tree
    Node node = Tree_root(tdp->tree);
    Stack_push(tdp->stack, START);                                      //push the start symbol to the stack
    while(Stack_size(tdp->stack) > 0) {                                 //while there are symbols in the stack
        int peek = Stack_peek(tdp->stack);
        if(G_is_nonterminal(peek)) {                                            //if the top of the stack is a nonterminal
            int next = tdp->table[peek][G_translate_next(tdp, false)];
            if(next > 0) {                                                          //if there is a valid next production
                char* label = malloc(2);                                                //create a new node
                label[0] = Stack_pop(tdp->stack); label[1] = '\0';                      //pop the production off the stack
                Node newChild = Node_new(node, label);
                Node_add_child(node, newChild);
                node = newChild;
                G_push_to_stack(tdp, next);                                             //push this production's symbols to the stack
            } else {
                printf("\n ** Error (no production defined for this combination, or missing end of input symbol '$')! **\n");
                return false;
            }
        } else if(peek == G_translate_next(tdp, true)) {                        //if the top of the stack is a terminal
            char* label = malloc(2);
            label[0] = Stack_pop(tdp->stack); label[1] = '\0';                      //pop the terminal off the stack
            Node newChild = Node_new(node, label);
            Node_add_child(node, newChild);
            node = newChild;
            TDP_consume(tdp);                                                           //consume one input character
        } else {
            printf("\n ** Error (unexpected character in input)! **\n");
            return false;
        }
    }
    printf("\n ** Successfully parsed! **\n");
    return true;
}

/*
 * "Consumes" character (increments the character index of the TDP by 1)
 */
void TDP_consume(TDP tdp) {
    tdp->index++;
}

/*
 * Returns the lookahead char (next character in the input string) without consuming anything
 */
char TDP_next(TDP tdp) {
    return tdp->input[tdp->index + 1];
}

/*
 * Translates the TDP's lookahead char into an enumerated terminal value
 * If raw is defined as false, the tdp->table index of that terminal is returned instead
 */
int G_translate_next(TDP tdp, bool raw) {
    int next = TDP_next(tdp);
    if(G_is_digit(next)) {
        next = raw ? NUMBER : NUMBER - nonterminals;
    } else if(next == '(') {
        next = raw ? LPAREN : LPAREN - nonterminals;
    } else if(next == ')') {
        next = raw ? RPAREN : RPAREN - nonterminals;
    } else if(next == '+') {
        next = raw ? PLUS : PLUS - nonterminals;
    } else if(next == '-') {
        next = raw ? MINUS : MINUS - nonterminals;
    } else if(next == '*') {
        next = raw ? MULTIPLY : MULTIPLY - nonterminals;
    } else if(next == '/') {
        next = raw ? DIVIDE : DIVIDE - nonterminals;
    } else if(next == '$') {
        next = raw ? BOTTOM : BOTTOM - nonterminals;
    }
    return next;
}

/*
 * Pushes the right-hand side of the production matching the given action to the stack.
 */
bool G_push_to_stack(TDP tdp, int action) {
    switch(action) {
        case 1:
            Stack_push(tdp->stack, BOTTOM);
            Stack_push(tdp->stack, EXPRESSION);
            break;
        case 2:
            Stack_push(tdp->stack, EPRIME);
            Stack_push(tdp->stack, TERM);
            break;
        case 3:
            Stack_push(tdp->stack, EPRIME);
            Stack_push(tdp->stack, TERM);
            Stack_push(tdp->stack, PLUS);
            break;
        case 4:
            Stack_push(tdp->stack, EPRIME);
            Stack_push(tdp->stack, TERM);
            Stack_push(tdp->stack, MINUS);
            break;
        case 5:
            /* "match" epsilon */
            break;
        case 6:
            Stack_push(tdp->stack, TPRIME);
            Stack_push(tdp->stack, FACTOR);
            break;
        case 7:
            Stack_push(tdp->stack, TPRIME);
            Stack_push(tdp->stack, FACTOR);
            Stack_push(tdp->stack, MULTIPLY);
            break;
        case 8:
            Stack_push(tdp->stack, TPRIME);
            Stack_push(tdp->stack, FACTOR);
            Stack_push(tdp->stack, DIVIDE);
            break;
        case 9:
            /* "match" epsilon */
            break;
        case 10:
            Stack_push(tdp->stack, RPAREN);
            Stack_push(tdp->stack, EXPRESSION);
            Stack_push(tdp->stack, LPAREN);
            break;
        case 11:
            Stack_push(tdp->stack, NUMBER);
            break;
        default:
            return false;
    }
    return true;
}

/*
 * Returns true if i is a nonterminal, or false if not
 */
bool G_is_nonterminal(int i) {
    if(i == START  || i == EXPRESSION ||
       i == EPRIME || i == TERM       ||
       i == TPRIME || i == FACTOR) {
        return true;
    }
    return false;
}

/*
 * Returns true if c is a digit character (0-9), or false if not
 */
bool G_is_digit(char c) {
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
 * Allocate and initialize a new parse table instance.
 * This table can be generated by hand using FIRST() and FOLLOWS() sets
 */
int** G_table() {
    int** table = malloc(nonterminals * sizeof(int*));
    for(int i = 0; i < nonterminals; i++) {
        table[i] = malloc(terminals * sizeof(int));
    }
    table[0][0] = 1;
    table[0][6] = 1;
    table[1][0] = 2;
    table[1][6] = 2;
    table[2][1] = 5;
    table[2][2] = 3;
    table[2][3] = 4;
    table[2][7] = 5;
    table[3][0] = 6;
    table[3][6] = 6;
    table[4][1] = 9;
    table[4][2] = 9;
    table[4][3] = 9;
    table[4][4] = 7;
    table[4][5] = 8;
    table[4][7] = 9;
    table[5][0] = 10;
    table[5][6] = 11;
    return table;
}

/*
 * Frees the parse table from memory
 */
void G_table_free(TDP tdp) {
    for(int i = 0; i < nonterminals; i++) {
        free(tdp->table[i]);
    }
    free(tdp->table);
}

/*
 * Frees the TDP from memory
 */
void TDP_free(TDP tdp) {
    Stack_free(tdp->stack);
    Tree_free(tdp->tree);
    G_table_free(tdp);
    free(tdp);
}
