#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

song* loadsongs(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return NULL;
    song* head = NULL;
    song* tail = NULL;
    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n' || line[0] == '\0'){
            continue;
        }
        song* s = song_txt_struct(line);
        if (!s){
            continue;
        }
        if (!head){
            head = tail = s;
        } else {
            tail->next = s;
            tail = s;
        }
    }
    fclose(f);
    return head;
}

playlist* loadplaylists(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f){
        return NULL;
    }
    playlist* head = NULL;
    playlist* tail = NULL;
    char line[2048];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '\n' || line[0] == '\0'){
            continue;
        }
        playlist* p = playlist_txt_struct(line);
        if (!p){
            continue;
        }
        if (!head){
            head = tail = p;
        } else {
            tail->next = p;
            tail = p;
        }
    }
    fclose(f);
    return head;
}

int saveplaylists(const char* path, const playlist* head) {
    FILE* f = fopen(path, "w");
    if (!f){
        return -1;
    }
    for (const playlist* p = head; p; p = p->next) {
        if (!p){
            continue;
        }
        fprintf(f, "%s|", p->name);
        playlistNode* cur = p->songs;
        while (cur) {
            fprintf(f, "%d", cur->song_id);
            if (cur->next){
                fputc(',', f);
            }
            cur = cur->next;
        }
        fputc('\n', f);
    }
    fclose(f);
    return 0;
}
