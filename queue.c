#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

queue* queuecrt(void) {
    queue* q = malloc(sizeof(queue));
    if (!q){
        return NULL;
    }
    q->head = q->current = NULL;
    q->size = 0;
    return q;
}

void queuefree(queue* q) {
    if (!q){
        return;
    }
    if (q->head){
        queueNode* cur = q->head->next;
        while (cur != q->head){
            queueNode* n = cur->next;
            free(cur);
            cur = n;
        }
        free(q->head);
    }
    free(q);
}

void queue_add_song(queue* q, song* s) {
    if (!q || !s){
        return;
    }
    queueNode* n = malloc(sizeof(queueNode));
    if (!n){
        return;
    }
    n->songptr = s;
    if (!q->head){
        q->head = n; n->next = n->prev = n; q->current = n; q->size = 1;
        return;
    }
    queueNode* tail = q->head->prev;
    tail->next = n; n->prev = tail;
    n->next = q->head; q->head->prev = n; q->size++;
}

void queuenxt(queue* q){
    if (q && q->current){
        q->current = q->current->next;
    }
}
void queueprev(queue* q) {
    if (q && q->current){
        q->current = q->current->prev;
    }
}

void queueshow(const queue* q) {
    if (!q || !q->head) {
        printf("(queue empty)\n");
        return;
    }
    const queueNode* cur = q->head; int idx = 0;
    do {
        int min = cur->songptr->duration / 60;
        int sec = cur->songptr->duration % 60;
        printf("%s%2d.   %-35s - %-25s %-40s (%d:%02d)%s\n", (cur==q->current)?">  ":"   ", ++idx, cur->songptr->title, cur->songptr->artist,cur->songptr->album, min, sec, (cur==q->current)?" <-- current":"");
        cur = cur->next;
    } while (cur != q->head);
}

void queue_remove_song(queue* q, int index) {
    if (!q || !q->head) {
        printf("queue empty\n");
        return;
    }

    if (index <= 0) {
        printf("Invalid song number in queue\n");
        return;
    }

    queueNode* cur = q->head;
    int pos = 1;

    while (pos < index && cur->next != q->head) {
        cur = cur->next;
        pos++;
    }

    if (pos != index) {
        printf("Invalid song number in queue\n");
        return;
    }

    const char* title  = cur->songptr->title;
    const char* artist = cur->songptr->artist;

    if (cur->next == cur) {
        free(cur);
        q->head = q->current = NULL;
        q->size = 0;
    } else {
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;

        if (q->head == cur)
            q->head = cur->next;
        if (q->current == cur)
            q->current = cur->next;

        free(cur);
        q->size--;
    }

    printf("Removed [%s - %s] from queue\n", title, artist);
}


