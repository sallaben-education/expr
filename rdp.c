//
//  rdp.c
//  expr
//
//  Created by sallaben on 10/4/17.
//

#include "rdp.h"

/*
 * RDP struct definition
 */
struct RDP {
    int index;                                      //character index measuring how much of the input has been evaluated
    const char* input;                              //stores the full input given to the RDP as a constant
};

/*
 * Allocate and initialize a new RDP instance
 */
RDP RDP_new(char* input) {
    RDP rdp = malloc(sizeof(RDP));                  //allocate memory for new RDP
    rdp->index = -1;                                //set its initial index to -1, signifying that the RDP has not yet processed any input
    rdp->input = input;                             //stores the full input expression
    return rdp;
}

/*
 * Processes an input using the given RDP and recursive-descent parsing,
 * starting by checking whether or not the input matches G_expression()
 */
void RDP_process(RDP rdp) {
    if(G_expression(rdp)) {
        if(G_next(rdp) == '\0') {
            printf("\n * Successfully parsed! *\n");
        } else {
            printf("\n * Error while reading end of expression! *\n");
        }
    } else {
        printf("\n * Error(s) while parsing expression! *\n");
    }
    
}

/*
 * "Consumes" character (increments the character index of the RDP by 1)
 */
void RDP_consume(RDP rdp) {
    rdp->index++;
}

/*
 * Returns the lookahead char (next character in the input string) without consuming anything
 */
char G_next(RDP rdp) {
    return rdp->input[rdp->index + 1];
}

/*
 * Matches an input character in the given RDP to a Digit
 */
bool G_digit(RDP rdp) {
    char c = G_next(rdp);
    if(c == '0' || c == '1' ||
       c == '2' || c == '3' ||
       c == '4' || c == '5' ||
       c == '6' || c == '7' ||
       c == '8' || c == '9') {
        RDP_consume(rdp);
        return true;
    }
    return false;
}

/*
 * Matches an input character in the given RDP to a Number
 */
bool G_number(RDP rdp) {
    if(G_digit(rdp)) {
        if(G_number_prime(rdp)) {
            return true;
        }
    }
    return false;
}

/*
 * Matches an input character in the given RDP to a Number' (a product of eliminating left-recursion)
 */
bool G_number_prime(RDP rdp) {
    if(G_number(rdp)) {
        return true;
    }
    return true;
}

/*
 * Matches an input character in the given RDP to a Factor
 */
bool G_factor(RDP rdp) {
    if(G_number(rdp)) {
        return true;
    } else if(G_next(rdp) == '(') {
        RDP_consume(rdp);
        if(G_expression(rdp)) {
            if(G_next(rdp) == ')') {
                RDP_consume(rdp);
                return true;
            }
        }
    }
    return false;
}

/*
 * Matches an input character in the given RDP to a Factor' (a product of eliminating left-recursion)
 */
bool G_factor_prime(RDP rdp) {
    switch(G_next(rdp)) {
        case '*':
            RDP_consume(rdp);
            if(G_factor(rdp)) {
                if(G_factor_prime(rdp)) {
                    return true;
                }
            } break;
        case '/':
            RDP_consume(rdp);
            if(G_factor(rdp)) {
                if(G_factor_prime(rdp)) {
                    return true;
                }
            } break;
        default: return true;
    }
    return false;
}

/*
 * Matches an input character in the given RDP to a Term
 */
bool G_term(RDP rdp) {
    if(G_factor(rdp)) {
        if(G_factor_prime(rdp)) {
            return true;
        }
    }
    return false;
}

/*
 * Matches an input character in the given RDP to a Term' (a product of eliminating left-recursion)
 */
bool G_term_prime(RDP rdp) {
    switch(G_next(rdp)) {
        case '+':
            RDP_consume(rdp);
            if(G_term(rdp)) {
                if(G_term_prime(rdp)) {
                    return true;
                }
            } break;
        case '-':
            RDP_consume(rdp);
            if(G_term(rdp)) {
                if(G_term_prime(rdp)) {
                    return true;
                }
            } break;
        default: return true;
    }
    return false;
}

/*
 * Matches an input character in the given RDP to an Expression
 */
bool G_expression(RDP rdp) {
    if(G_term(rdp)) {
        if(G_term_prime(rdp)) {
            return true;
        }
    }
    return false;
}

/*
 * Frees the RDP from memory
 */
void RDP_free(RDP rdp) {
    free(rdp);
}
