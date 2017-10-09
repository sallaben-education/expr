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

extern bool G_is_digit(char c);

extern int** G_table(void);
    
extern void G_table_free(TDP tdp);

extern void TDP_free(TDP tdp);

#endif /* tdp_h */