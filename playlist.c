#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "playlist.h"

playlist* playlistcrt(const char* name) {
    playlist* p = malloc(sizeof(playlist));
    if (!p){
        return NULL;
    }
    strncpy(p->name, name, 255);
    p->name[255] = '\0';
    p->songs = NULL;
    p->next = NULL;
    return p;
}

void playlistfree(playlist* head) {
    while (head) {
        playlist* t = head;
        head = head->next;
        playlistNode* s = t->songs;
        while (s){
            playlistNode* n = s; s = s->next;
            free(n);
        }
        free(t);
    }
}

playlist* playlistfind(playlist* head, const char* name) {
    for (; head; head = head->next){
        if (strcmp(head->name, name) == 0){
            return head;
        }
    }
    return NULL;
}

void playlist_add_song(playlist* p, int song_id) {
    if (!p){
        return;
    }
    playlistNode* n = malloc(sizeof(playlistNode));
    if (!n){
        return;
    }
    n->song_id = song_id;
    n->next = NULL;
    if (!p->songs) {
        p->songs = n;
        return;
    }
    playlistNode* cur = p->songs;
    while (cur->next){
        cur = cur->next;
    }
    cur->next = n;
}

void playlist_remove_song(playlist* p, int song_id) {
    if (!p) return;
    playlistNode* cur = p->songs;
    playlistNode* prev = NULL;
    while (cur) {
        if (cur->song_id == song_id) {
            if (prev){
                prev->next = cur->next;
            } else{
                p->songs = cur->next;
            }
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

playlist* playlist_txt_struct(const char* line) {
    if (!line){
        return NULL;
    }
    char name[256] = {0};
    if (sscanf(line, "%255[^|]|", name) < 1){
        return NULL;
    }
    const char* p = strchr(line, '|');
    if (!p){
        return NULL;
    }
    p++;
    playlist* pl = playlistcrt(name);
    if (!pl){
        return NULL;
    }
    char ids[1024];
    strncpy(ids, p, 1023);
    ids[1023] = '\0';
    char* tok = strtok(ids, ",\n");
    while (tok) {
        int id = atoi(tok);
        if (id > 0) playlist_add_song(pl, id);
        tok = strtok(NULL, ",\n");
    }
    return pl;
}
