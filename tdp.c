//
//  tdp.c
//  expr
//
//  Created by sallaben on 10/8/17.
//

#include "tdp.h"
#include "stack.h"

int nonterminals = 6;
int terminals = 8;

/*
 * TDP struct definition
 */
struct TDP {
    int index;                                              //character index measuring how much of the input has been evaluated
    const char* input;                                      //stores the full input given to the TDP as a constant
    Stack stack;                                            //the stack component of the table driven parser (making this a pushdown automaton)
    int** table;                                            //a parse table defining the grammar rules for this TDP
};

/*
 * Allocate and initialize a new TDP instance
 */
TDP TDP_new(char* input) {
    TDP tdp = malloc(sizeof(TDP));                          //allocate memory for new TDP
    tdp->index = -1;                                        //set its initial index to -1, signifying that the TDP has not yet processed any input
    tdp->input = input;                                     //stores the full input expression
    tdp->stack = Stack_new();                               //initialize a new, empty Stack
    tdp->table = G_table();                                 //stores the parse table
    return tdp;
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
 * Allocate and initialize a new parse table instance
 */
int** G_table() {
    int** table = malloc(nonterminals * sizeof(int*));
    for(int i = 0; i < nonterminals; i++) {
        table[i] = malloc(terminals * sizeof(int));
    }
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
    free(tdp->stack);
    G_table_free(tdp);
    free(tdp);
}
