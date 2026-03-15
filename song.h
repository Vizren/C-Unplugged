#ifndef SONG_H
#define SONG_H

#define MAX_STR 256

typedef struct song {
    int id;
    char title[MAX_STR];
    char artist[MAX_STR];
    char album[MAX_STR];
    int duration;
    struct song* next;
} song;

song* songcrt(int id, const char* title, const char* artist, const char* album, int duration);
void songfree(song* head);
song* song_txt_struct(const char* line);
song* findsong(song* head, int id);

#endif
