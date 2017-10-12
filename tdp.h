//
//  tdp.h
//  expr
//
//  Created by sallaben on 10/8/17.
//

#ifndef tdp_h
#define tdp_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct TDP *TDP;

extern TDP TDP_new(char* input);

extern bool TDP_process(TDP tdp);

extern void TDP_consume(TDP tdp);

extern char TDP_next(TDP tdp);

extern int G_translate_next(TDP tdp, bool raw);

extern bool G_push_to_stack(TDP tdp, int action);

extern bool G_is_nonterminal(int i);

extern bool G_is_digit(char c);

extern int** G_table(void);
    
extern void G_table_free(TDP tdp);

extern void TDP_free(TDP tdp);

#endif /* tdp_h */
