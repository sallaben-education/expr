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

/*
 * Get string input from user followed by RETURN/ENTER.
 */
char* get_input() {
    static char input[64];
    printf("\n Please enter an input expression");
    printf("\n (64 characters max): ");
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

void RDP_execute() {
    char* input = get_input();
    RDP rdp = RDP_new(input);
    RDP_process(rdp);
    RDP_free(rdp);
}

void TDP_execute() {
    char* input = get_input();
    TDP tdp = TDP_new(input);
    TDP_process(tdp);
    TDP_free(tdp);
}

void PT_evaluate() {
    //choose rdp or tdp to generate parse tree
    //evaluate for result of parse tree
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
        printf("  3. Evaluate parse trees\n");
        printf("  4. Quit\n");
        int choice = -1;
        while(choice == -1) {                                               //until there is a valid integer chosen
            printf("\n Select an option > ");
            scanf("%i", &choice);                                               //extract integer from stdin
            char c; while((c = getchar()) != '\n');                             //clear remaining chars in stdin
        }
        if((choice >= 1) && (choice <= 4)) {                                //checks whether or not the choice is a valid one
            switch(choice) {
                case 1: RDP_execute(); break;                                   //run the recursive-descent parser
                case 2: TDP_execute(); break;                                   //run the table driven parser
                case 3: PT_evaluate(); break;                                   //create and evaluate a parse tree
                case 4: escape = true; break;                                   //exit the menu loop
            }
        } else {
            printf("\nI didn't understand your input. Please try again!\n");
        }
    }
    printf("\n*** Application terminated by user ***\n");
    printline();
    printf("\n");
    return 0;
}
