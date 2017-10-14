//
//  rdp.h
//  expr
//
//  Created by sallaben on 10/4/17.
//

#ifndef rdp_h
#define rdp_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "node.h"
#include "tree.h"

typedef struct RDP *RDP;

extern RDP RDP_new(char* input);

extern bool RDP_process(RDP rdp);

extern void RDP_consume(RDP rdp);

extern char RDP_next(RDP rdp);

extern bool G_digit(RDP rdp, Node node);

extern bool G_number(RDP rdp, Node node);

extern bool G_number_prime(RDP rdp, Node node);

extern bool G_factor(RDP rdp, Node node);

extern bool G_factor_prime(RDP rdp, Node node);

extern bool G_term(RDP rdp, Node node);

extern bool G_term_prime(RDP rdp, Node node);

extern bool G_expression_prime(RDP rdp, Node node);

extern bool G_expression(RDP rdp, Node node);

extern Tree RDP_get_tree(RDP rdp);

extern void RDP_free(RDP rdp);

#endif /* rdp_h */
