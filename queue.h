//
//  queue.h
//  expr
//
//  Created by sallaben on 10/14/17.
//

#ifndef queue_h
#define queue_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Queue *Queue;

extern Queue Queue_new(bool integers);

extern void Queue_enqueue(Queue queue, int i);

extern int Queue_dequeue(Queue queue);

extern int Queue_peek(Queue queue);

extern int Queue_size(Queue queue);

extern void Queue_print(Queue Queue);

extern bool Queue_shift_left(Queue queue);

#endif /* queue_h */

