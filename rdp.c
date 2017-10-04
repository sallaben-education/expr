//
//  rdp.c
//  expr
//
//  Created by sallaben on 10/4/17.
//

#include "rdp.h"

struct RDP {
    char symbol;
};

RDP RDP_new() {
    RDP rdp = malloc(sizeof(RDP));                  //allocate memory for new RDP
    rdp->symbol = '\\';                             //set its initial symbol to a placeholder (escaped backslash),
    return rdp;                                         //signifying that the RDP has not yet processed any input
}

void RDP_free(RDP rdp) {
    free(rdp);
}

void update_symbol(RDP rdp) {
    //rdp.update_symbol() TODO: consume input (but from where?)
}
