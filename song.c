#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "song.h"

song* songcrt(int id, const char* title, const char* artist, const char* album, int duration) {
    song* s = malloc(sizeof(song));
    if (!s){
        return NULL;
    }
    s->id = id;
    strncpy(s->title, title, MAX_STR - 1);
    s->title[MAX_STR - 1] = '\0';
    strncpy(s->artist, artist, MAX_STR - 1);
    s->artist[MAX_STR - 1] = '\0';
    strncpy(s->album, album, MAX_STR - 1);
    s->album[MAX_STR - 1] = '\0';
    s->duration = duration;
    s->next = NULL;
    return s;
}

void songfree(song* head) {
    while (head) {
        song* t = head;
        head = head->next;
        free(t);
    }
}

song* song_txt_struct(const char* line) {
    if (!line) return NULL;
    int id = 0, duration = 0;
    char title[MAX_STR], artist[MAX_STR], album[MAX_STR];
    if (sscanf(line, "%d|%255[^|]|%255[^|]|%255[^|]|%d",&id, title, artist, album, &duration) < 5){
        return NULL;
    }
    return songcrt(id, title, artist, album, duration);
}

song* findsong(song* head, int id) {
    for (song* s = head; s; s = s->next){
        if (s->id == id){
            return s;
        }
    }
    return NULL;
}
