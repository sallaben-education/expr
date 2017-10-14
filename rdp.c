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
    int index;                          //character index measuring how much of the input has been evaluated
    char* input;                        //stores the full input given to the RDP as a constant
    Tree tree;                          //stores the parse tree object for this RDP
    bool epsilon;                       //if true, the last production matched was with the symbol epsilon (useful for creating the tree)
};

/*
 * Allocate and initialize a new RDP instance
 */
RDP RDP_new(char* input) {
    RDP rdp = malloc(sizeof(struct RDP));               //allocate memory for new RDP
    rdp->index = -1;                                    //set its initial index to -1, signifying that the RDP has not yet processed any input
    rdp->input = input;                                 //stores the full input expression
    rdp->epsilon = false;
    return rdp;
}

/*
 * Processes an input using the given RDP and recursive-descent parsing,
 * starting by checking whether or not the input matches G_expression()
 */
bool RDP_process(RDP rdp) {
    rdp->tree = Tree_new(Node_new(NULL, "EXPRESSION"));
    if(G_expression(rdp, Tree_root(rdp->tree))) {
        if(RDP_next(rdp) == '$') {
            printf("\n ** Successfully parsed! **\n");
            return true;
        } else {
            printf("\n ** Error while reading end of expression! **\n");
            return false;
        }
    } else {
        printf("\n ** Error(s) while parsing expression! **\n");
        return false;
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
char RDP_next(RDP rdp) {
    return rdp->input[rdp->index + 1];
}

/*
 * Matches an input character in the given RDP to a Digit
 */
bool G_digit(RDP rdp, Node node) {
    char c = RDP_next(rdp);
    if(c == '0' || c == '1' ||
       c == '2' || c == '3' ||
       c == '4' || c == '5' ||
       c == '6' || c == '7' ||
       c == '8' || c == '9') {
        char* label = malloc(2);
        label[0] = c; label[1] = '\0';
        Node charNode = Node_new(node, label);
        Node_add_child(node, charNode);
        RDP_consume(rdp);
        return true;
    }
    return false;
}

/*
 * Matches an input character in the given RDP to a Number
 */
bool G_number(RDP rdp, Node node) {
    Node digitNode = Node_new(node, "DIGIT");
    Node numberPrimeNode = Node_new(node, "NUMBERPRIME");
    if(G_digit(rdp, digitNode)) {
        Node_add_child(node, digitNode);
        if(G_number_prime(rdp, numberPrimeNode)) {
            if(!rdp->epsilon) {
                Node_add_child(node, numberPrimeNode);
            } else {
                rdp->epsilon = false;
            }
            return true;
        }
    }
    return false;
}

/*
 * Matches an input character in the given RDP to a Number' (a product of eliminating left-recursion)
 */
bool G_number_prime(RDP rdp, Node node) {
    Node numberNode = Node_new(node, "NUMBER");
    if(G_number(rdp, numberNode)) {
        Node_add_child(node, numberNode);
        return true;
    }
    rdp->epsilon = true;
    return true;
}

/*
 * Matches an input character in the given RDP to a Factor
 */
bool G_factor(RDP rdp, Node node) {
    Node numberNode = Node_new(node, "NUMBER");
    Node lparenNode = Node_new(node, "(");
    Node expressionNode = Node_new(node, "EXPRESSION");
    Node rparenNode = Node_new(node, ")");
    if(G_number(rdp, numberNode)) {
        Node_add_child(node, numberNode);
        return true;
    } else if(RDP_next(rdp) == '(') {
        Node_add_child(node, lparenNode);
        RDP_consume(rdp);
        if(G_expression(rdp, expressionNode)) {
            Node_add_child(node, expressionNode);
            if(RDP_next(rdp) == ')') {
                Node_add_child(node, rparenNode);
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
bool G_factor_prime(RDP rdp, Node node) {
    Node multiplyNode = Node_new(node, "*");
    Node divideNode = Node_new(node, "/");
    Node factorNode = Node_new(node, "FACTOR");
    Node factorPrimeNode = Node_new(factorNode, "FACTORPRIME");
    switch(RDP_next(rdp)) {
        case '*':
            Node_add_child(node, multiplyNode);
            RDP_consume(rdp);
            if(G_factor(rdp, factorNode)) {
                Node_add_child(node, factorNode);
                if(G_factor_prime(rdp, factorPrimeNode)) {
                    if(!rdp->epsilon) {
                        Node_add_child(node, factorPrimeNode);
                    } else {
                        rdp->epsilon = false;
                    }
                    return true;
                }
            } break;
        case '/':
            Node_add_child(node, divideNode);
            RDP_consume(rdp);
            if(G_factor(rdp, factorNode)) {
                Node_add_child(node, factorNode);
                if(G_factor_prime(rdp, factorPrimeNode)) {
                    if(!rdp->epsilon) {
                        Node_add_child(node, factorPrimeNode);
                    } else {
                        rdp->epsilon = false;
                    }
                    return true;
                }
            } break;
        default: rdp->epsilon = true; return true;
    }
    return false;
}

/*
 * Matches an input character in the given RDP to a Term
 */
bool G_term(RDP rdp, Node node) {
    Node factorNode = Node_new(node, "FACTOR");
    Node factorPrimeNode = Node_new(node, "FACTORPRIME");
    if(G_factor(rdp, factorNode)) {
        Node_add_child(node, factorNode);
        if(G_factor_prime(rdp, factorPrimeNode)) {
            if(!rdp->epsilon) {
                Node_add_child(node, factorPrimeNode);
            } else {
                rdp->epsilon = false;
            }
            return true;
        }
    }
    return false;
}

/*
 * Matches an input character in the given RDP to a Term' (a product of eliminating left-recursion)
 */
bool G_term_prime(RDP rdp, Node node) {
    Node plusNode = Node_new(node, "+");
    Node minusNode = Node_new(node, "-");
    Node termNode = Node_new(node, "TERM");
    Node termPrimeNode = Node_new(termNode, "TERMPRIME");
    switch(RDP_next(rdp)) {
        case '+':
            Node_add_child(node, plusNode);
            RDP_consume(rdp);
            if(G_term(rdp, termNode)) {
                Node_add_child(node, termNode);
                if(G_term_prime(rdp, termPrimeNode)) {
                    if(!rdp->epsilon) {
                        Node_add_child(node, termPrimeNode);
                    } else {
                        rdp->epsilon = false;
                    }
                    return true;
                }
            } break;
        case '-':
            Node_add_child(node, minusNode);
            RDP_consume(rdp);
            if(G_term(rdp, termNode)) {
                Node_add_child(node, termNode);
                if(G_term_prime(rdp, termPrimeNode)) {
                    if(!rdp->epsilon) {
                        Node_add_child(node, termPrimeNode);
                    } else {
                        rdp->epsilon = false;
                    }
                    return true;
                }
            } break;
        default: rdp->epsilon = true; return true;
    }
    return false;
}

/*
 * Matches an input character in the given RDP to an Expression
 */
bool G_expression(RDP rdp, Node node) {
    Node termNode = Node_new(node, "TERM");
    Node termPrimeNode = Node_new(node, "TERMPRIME");
    if(G_term(rdp, termNode)) {
        Node_add_child(node, termNode);
        if(G_term_prime(rdp, termPrimeNode)) {
            if(!rdp->epsilon) {
                Node_add_child(node, termPrimeNode);
            } else {
                rdp->epsilon = false;
            }
            return true;
        }
    }
    return false;
}

/*
 * Returns the given RDP's tree
 */
Tree RDP_get_tree(RDP rdp) {
    return rdp->tree;
}

/*
 * Frees the RDP from memory
 */
void RDP_free(RDP rdp) {
    free(rdp);
}
