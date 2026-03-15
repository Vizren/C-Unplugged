#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "song.h"

typedef struct playlistNode {
    int song_id;
    struct playlistNode* next;
} playlistNode;

typedef struct playlist {
    char name[256];
    playlistNode* songs;
    struct playlist* next;
} playlist;

playlist* playlistcrt(const char* name);
void playlistfree(playlist* head);
playlist* playlistfind(playlist* head, const char* name);

void playlist_add_song(playlist* p, int song_id);
void playlist_remove_song(playlist* p, int song_id);

playlist* playlist_txt_struct(const char* line);

#endif