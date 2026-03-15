#ifndef STORAGE_H
#define STORAGE_H

#include "song.h"
#include "playlist.h"

song* loadsongs(const char* path);
playlist* loadplaylists(const char* path);
int saveplaylists(const char* path, const playlist* head);

#endif // STORAGE_H
