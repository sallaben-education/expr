//
//  queue.c
//  expr
//
//  Created by sallaben on 10/14/17.
//

#include "queue.h"

/*
 * Queue struct definition
 */
struct Queue {
    int list[64];
    int size;
};

/*
 * Allocate and initialize a new instance of a queue (with a limit of 64 integers).
 */
Queue Queue_new(void) {
    Queue queue = malloc(sizeof(struct Queue));
    queue->size = 0;
    return queue;
}

/*
 * Adds a new integer to the back of the queue
 */
void Queue_enqueue(Queue queue, int i) {
    if(queue->size < 64) {
        queue->list[queue->size] = i;
        queue->size++;
    } else {
        printf("Cannot add: Queue has reached its maximum size!");
    }
}

/*
 * Returns and removes the current front integer of the queue
 */
int Queue_dequeue(Queue queue) {
    int i = queue->list[0];
    Queue_shift_left(queue);
    queue->size--;
    return i;
}

/*
 * Returns the current top integer of the queue
 */
int Queue_peek(Queue queue) {
    if(queue->size > 0) {
        return queue->list[0];
    } else {
        return -1;
    }
}

/*
 * Returns the current integer size of the queue
 */
int Queue_size(Queue queue) {
    return queue->size;
}

/*
 * Prints the given queue in order with attention to the
 */
void Queue_print(Queue queue) {
    int size = Queue_size(queue);
    printf("\n\nPrinting Queue of size %d", size);
    if(size > 0) {
        for(int i = 0; i < size; i++) {
            if(i == 0) {
                printf("\n[%d]", queue->list[0]);
            } else {
                printf("\n%d", queue->list[i]);
            }
        }
    } else {
        printf("\nNothing to print!");
    }
}

/*
 * "Shifts" the given queue 1 space to the left, removing space left behind from Queue_pop()
 */
bool Queue_shift_left(Queue queue) {
    if(queue->size <= 0) {
        return false;
    }
    for(int i = 1; i < queue->size; i++) {
        queue->list[i - 1] = queue->list[i];
    }
    return true;
}

/*
 * Frees the Queue from memory
 */
void Queue_free(Queue queue) {
    free(queue);
}
