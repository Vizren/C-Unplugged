#ifndef QUEUE_H
#define QUEUE_H

#include "song.h"

typedef struct queueNode {
    song* songptr;
    struct queueNode* next;
    struct queueNode* prev;
} queueNode;

typedef struct queue {
    queueNode* head;
    queueNode* current;
    int size;
} queue;

queue* queuecrt(void);
void queuefree(queue* q);
void queue_add_song(queue* q, song* s);

void queuenxt(queue* q);
void queueprev(queue* q);
void queueshow(const queue* q);
void queue_remove_song(queue* q, int song_id);

#endif
