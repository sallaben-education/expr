//
//  main.c
//  expr
//
//  Created by sallaben on 10/1/17.
//

#include <string.h>
#include "node.h"
#include "stack.h"
#include "rdp.h"
#include "tdp.h"
#include "tree.h"
#include "queue.h"

/*
 * Get string input from user followed by RETURN/ENTER.
 */
char* get_input() {
    static char input[64];
    printf("\n Please enter an arithmetic expression:");
    printf("\n   - Valid chars: ([0-9]+-*/)$");
    printf("\n   - Append $ to mark end of input");
    printf("\n   - 64 chars max");
    printf("\n\n Enter an input > ");
    fgets(input, 64, stdin);
    char *c = strstr(input, "\n");
    *c = '\0';
    return input;
}

/*
 * Prints a line of hyphens (useful for formatting).
 */
void printline() {
    printf("\n----------------------------------------\n");
}

/*
 * Asks the user for input and evaluates it using a recursive-descent parser.
 */
void RDP_execute() {
    char* input = get_input();
    RDP rdp = RDP_new(input);
    RDP_process(rdp);
    RDP_free(rdp);
}

/*
 * Asks the user for input and evaluates it using a table-driven parser.
 */
void TDP_execute() {
    char* input = get_input();
    TDP tdp = TDP_new(input);
    TDP_process(tdp);
    TDP_free(tdp);
}

/*
 * Prints and evaluates the parse tree produced by the parser.
 * Uses the Shunting-Yard algorithm and a Stack to evaluate the parse tree.
 */
void PT_evaluate(void) {
    char* input = get_input();
    RDP rdp = RDP_new(input);
    Tree tree = NULL;
    if(RDP_process(rdp)) {
        tree = RDP_get_tree(rdp);
        Tree_print(tree);
        Tree_parse(tree);
        int result = Tree_evaluate(tree);
        if(result != -1) {
            printf("\n Parse tree evaluates to: %d\n", result);
        } else {
            printf("\n Parse tree could not be successfully evaluated!\n");
        }
    }
    Tree_free(tree);
    RDP_free(rdp);
}

/*
 * Main method of expr
 */
int main(int argc, const char * argv[]) {
    bool escape = false;
    while(!escape) {                                                    //loop until escape is true
        printline();
        printf("           Parser & Evaluator");                            //menu formatting
        printline();
        printf("\n");
        printf("[Main Menu]\n");
        printf("  1. Recursive-descent parser\n");
        printf("  2. Table-driven parser\n");
        printf("  3. View and evaluate parse tree\n");
        printf("  4. Quit\n");
        int choice = -1;
        while(choice == -1) {                                               //until there is a valid integer chosen
            printf("\n Select an option > ");
            scanf("%i", &choice);                                               //extract integer from stdin
            char c; while((c = getchar()) != '\n');                             //clear remaining chars in stdin
        }
        if((choice >= 1) && (choice <= 5)) {                                //checks whether or not the choice is a valid one
            switch(choice) {
                case 1: RDP_execute(); break;                                   //run the recursive-descent parser
                case 2: TDP_execute(); break;                                   //create and evaluate a parse tree
                case 3: PT_evaluate(); break;                                   //run the table driven parser
                case 4: escape = true; break;                                   //exit the menu loop
            }
        } else {
            printf("\nI didn't understand your input. Please try again!\n");
        }
    }
    printf("\n** Application terminated by user **\n");
    printline();
    printf("\n");
    return 0;
}
