//
//  stack.h
//  expr
//
//  Created by sallaben on 10/9/17.
//

#ifndef stack_h
#define stack_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Stack *Stack;

extern Stack Stack_new(void);

extern void Stack_push(Stack stack, int i);

extern int Stack_pop(Stack stack);

extern int Stack_peek(Stack stack);

extern int Stack_size(Stack stack);

extern void Stack_print(Stack stack);

extern bool Stack_shift_left(Stack stack);

extern bool Stack_shift_right(Stack stack);

extern void Stack_free(Stack stack);

#endif /* stack_h */
