#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
#include "storage.h"
#include "log.h"

int playlist_get_song_id_at(playlist *p, int pos) {
    if (!p || pos <= 0) return -1;

    playlistNode *cur = p->songs;
    int i = 1;
    while (cur && i < pos) {
        cur = cur->next;
        i++;
    }
    if (!cur) return -1;
    return cur->song_id;
}
playlist* playlist_get_at_index(playlist *head, int pos) {
    if (pos <= 0) return NULL;
    int i = 1;
    playlist *p = head;
    while (p && i < pos) {
        p = p->next;
        i++;
    }
    return p;
}

int playlist_contains_song(playlist *p, int id) {
    if (!p) return 0;
    playlistNode *cur = p->songs;
    while (cur) {
        if (cur->song_id == id) return 1;
        cur = cur->next;
    }
    return 0;
}

void listsongs(song* songs) {
    for (song* s = songs; s; s = s->next) {
        int min = s->duration / 60;
        int sec = s->duration % 60;
        printf("    %2d |   %-35s | %-25s | %-40s| %d:%02d\n",s->id, s->title, s->artist, s->album, min, sec);
    }
}

void listplaylists(playlist* head) {
    if (!head){
        printf("(no playlists)\n");
        return;
    }
    int i=1;
    for (playlist* p = head; p; p = p->next){
        printf("%2d. %s\n",i, p->name);
        i++;
    }
}

void viewplaylist(playlist* head, song* songs, const char* name) {
    playlist* p = playlistfind(head, name);
    if (!p){
        printf("playlist not found\n");
        return;
    }
    playlistNode* cur = p->songs;
    if (!cur){
        printf("(empty)\n");
        return;
    }
    int i=1;
    while (cur){
        song* s = findsong(songs, cur->song_id);
        if (s){
            int min = s->duration / 60;
            int sec = s->duration % 60;
            printf("    %2d.   %-35s - %-25s %-40s     %d:%02d\n",i, s->title, s->artist, s->album, min, sec);
        } else{
            printf("[id=%d] (missing)\n", cur->song_id);
        }
        cur = cur->next;
        i++;
    }
}

void createplaylist(playlist** playlists, const char* name) {
    if (playlistfind(*playlists, name)) {
        printf("playlist already exists\n");
        return;
    }
    playlist* p = playlistcrt(name);
    p->next = *playlists;
    *playlists = p;
    printf("created playlist '%s'\n", name);
}

void queue_add_playlist(playlist* playlists, const char* name, queue* q, song* songs) {
    playlist* p = playlistfind(playlists, name);
    if (!p) { printf("playlist not found\n"); return; }
    playlistNode* cur = p->songs;
    while (cur) {
        song* s = findsong(songs, cur->song_id);
        if (s) queue_add_song(q, s);
        cur = cur->next;
    }
    printf("added playlist '%s' to queue\n", name);
}

void cli_run(song** song_list, playlist** playlists, queue* q) {
    char line[1024];

    while (1) {
        printf("C-Unplugged> ");
        if (!fgets(line, sizeof(line), stdin)) break;
        line[strcspn(line, "\n")] = '\0';
        if (strlen(line) == 0) continue;

        mylog(line);

        int cmd;
        if (sscanf(line, "%d", &cmd) != 1) {
            printf("Invalid command. Type 0 for help.\n\n");
            continue;
        }

        switch (cmd) {
        case 0: //help
            printf("Commands (enter the number):\n");
            printf("     0  -  Help\n");
            printf("     1  -  List Songs\n");
            printf("     2  -  Add Song to Playlist\n");
            printf("     3  -  List Playlists\n");
            printf("     4  -  Create Playlist\n");
            printf("     5  -  View Playlist\n");
            printf("     6  -  Remove Song from Playlist\n");
            printf("     7  -  Add Song to Queue\n");
            printf("     8  -  Add Playlist to Queue\n");
            printf("     9  -  View Queue\n");
            printf("    10  -  Play\n");
            printf("    11  -  Next\n");
            printf("    12  -  Prev\n");
            printf("    13  -  Remove Song from Queue\n");
            printf("    14  -  Show Command History\n");
            printf("    15  -  Exit\n\n");
            break;

        case 15: //exit
            printf("Exiting...\n");
            return;

        case 1: //list songs
            listsongs(*song_list);
            printf("\n");
            break;

        case 3: //list playlists
            listplaylists(*playlists);
            printf("\n");
            break;

        case 4: { //create playlist
            char name[256];
            printf("Enter Playlist Name: ");
            if (!fgets(name, sizeof(name), stdin)) break;
            name[strcspn(name, "\n")] = '\0';

            playlist* p = playlistfind(*playlists, name);
            if (!p) {
                createplaylist(playlists, name);
                saveplaylists("playlists.txt", *playlists);
            } else {
                printf("Playlist already exists\n");
                printf("To add songs to playlist use Command '2'\n");
            }
            printf("\n");
            break;
        }

        case 2: { //add song to playlist
            int pindex;
            int id;

            if (!*playlists) {
                printf("No playlists. Create one first.\n\n");
                break;
            }

            listplaylists(*playlists);
            printf("Enter Playlist No: ");
            if (!fgets(line, sizeof(line), stdin)) break;
            if (sscanf(line, "%d", &pindex) < 1 || pindex <= 0) {
                printf("usage: <playlist-number must be positive>\n\n");
                break;
            }

            playlist* p = playlist_get_at_index(*playlists, pindex);
            if (!p) {
                printf("Invalid playlist number\n\n");
                break;
            }

            listsongs(*song_list);
            printf("Enter song ID: ");
            if (!fgets(line, sizeof(line), stdin)) break;
            if (sscanf(line, "%d", &id) < 1) {
                printf("usage: <song-id must be a number>\n");
                break;
            }

            song* s = findsong(*song_list, id);
            if (!s) {
                printf("ERROR: Invalid Song ID\n\n");
                break;
            }

            playlist_add_song(p, id);
            saveplaylists("playlists.txt", *playlists);
            printf("[%s - %s] added to %s\n\n",
                s->title, s->artist, p->name);
            break;
        }

        case 5: { //view playlist
            int pindex;

            if (!*playlists) {
                printf("No playlists.\n\n");
                break;
            }

            listplaylists(*playlists);
            printf("Enter Playlist No: ");
            if (!fgets(line, sizeof(line), stdin)) break;
            if (sscanf(line, "%d", &pindex) < 1 || pindex <= 0) {
                printf("usage: <playlist-number must be positive>\n\n");
                break;
            }

            playlist* p = playlist_get_at_index(*playlists, pindex);
            if (!p) {
                printf("Invalid playlist number\n\n");
                break;
            }

            viewplaylist(*playlists, *song_list, p->name);
            printf("\n");
            break;
        }

        case 6: { //remove song from playlist (playlist by number)
            int pindex;
            int pos;

            if (!*playlists) {
                printf("No playlists.\n\n");
                break;
            }

            listplaylists(*playlists);
            printf("Enter Playlist No: ");
            if (!fgets(line, sizeof(line), stdin)) break;
            if (sscanf(line, "%d", &pindex) < 1 || pindex <= 0) {
                printf("usage: <playlist-number must be positive>\n\n");
                break;
            }

            playlist* p = playlist_get_at_index(*playlists, pindex);
            if (!p) {
                printf("Invalid playlist number\n\n");
                break;
            }

            viewplaylist(*playlists, *song_list, p->name);

            printf("Enter Song No: ");
            if (!fgets(line, sizeof(line), stdin)) break;
            if (sscanf(line, "%d", &pos) < 1) {
                printf("usage: <song-number must be positive>\n\n");
                break;
            }

            int song_id = playlist_get_song_id_at(p, pos);
            if (song_id == -1) {
                printf("Invalid Song No\n\n");
                break;
            }

            song* s = findsong(*song_list, song_id);
            playlist_remove_song(p, song_id);
            saveplaylists("playlists.txt", *playlists);
            printf("[%s - %s] removed from %s\n\n",
                s->title, s->artist, p->name);
            break;
        }

        case 8: { //add playlist to queue (choose playlist by number)
            int pindex;

            if (!*playlists) {
                printf("No playlists.\n\n");
                break;
            }

            listplaylists(*playlists);
            printf("Enter Playlist No: ");
            if (!fgets(line, sizeof(line), stdin)) break;
            if (sscanf(line, "%d", &pindex) < 1 || pindex <= 0) {
                printf("usage: <playlist-number must be a positive integer>\n\n");
                break;
            }

            playlist* p = playlist_get_at_index(*playlists, pindex);
            if (!p) {
                printf("Invalid playlist number\n\n");
                break;
            }

            queue_add_playlist(*playlists, p->name, q, *song_list);
            printf("\n");
            break;
        }

        case 7: { //add song to queue
            int id;
            listsongs(*song_list);
            printf("Enter Song ID to add to Queue: ");
            if (!fgets(line, sizeof(line), stdin)) break;
            if (sscanf(line, "%d", &id) < 1) {
                printf("usage: <song-id must be a number>\n");
                break;
            }

            song* s = findsong(*song_list, id);
            if (!s) {
                printf("song not found\n\n");
                break;
            }
            queue_add_song(q, s);
            printf("Added [%s - %s] to queue\n\n", s->title, s->artist);
            break;
        }

        case 9: //show queue
            queueshow(q);
            printf("\n");
            break;

        case 10: { //play curr song
            if (!q || !q->current) {
                printf("queue empty\n\n");
                break;
            }
            song* s = q->current->songptr;
            int min = s->duration / 60;
            int sec = s->duration % 60;
            printf("Now playing: [%d] %s - %s (%d:%02d)\n\n",
                   s->id, s->title, s->artist, min, sec);
            break;
        }

        case 11: { //next
            if (!q || !q->current) {
                printf("Queue empty\n\n");
                break;
            }
            queuenxt(q);
            if (!q->current) {
                printf("Queue empty\n\n");
                break;
            }
            printf("moved to next\n");
            song* s = q->current->songptr;
            int min = s->duration / 60;
            int sec = s->duration % 60;
            printf("Now playing: [%d] %s - %s (%d:%02d)\n\n",
                   s->id, s->title, s->artist, min, sec);
            break;
        }

        case 12: { //prev
            if (!q || !q->current) {
                printf("Queue empty\n\n");
                break;
            }
            queueprev(q);
            if (!q->current) {
                printf("Queue empty\n\n");
                break;
            }
            printf("moved to prev\n");
            song* s = q->current->songptr;
            int min = s->duration / 60;
            int sec = s->duration % 60;
            printf("Now playing: [%d] %s - %s (%d:%02d)\n\n",
                   s->id, s->title, s->artist, min, sec);
            break;
        }

        case 13: { //remove song from queue
            int index;

            queueshow(q);
            printf("Enter Song No: ");
            if (!fgets(line, sizeof(line), stdin)) break;
            if (sscanf(line, "%d", &index) < 1 || index <= 0) {
                printf("usage: <song-number must be a positive integer>\n\n");
                break;
            }
            queue_remove_song(q, index);
            printf("\n");
            break;
        }

        case 14: //show cmd history
            showhistory("command_history.txt");
            printf("\n");
            break;

        default:
            printf("UNKNOWN COMMAND NUMBER\n");
            printf("Type 0 for help.\n\n");
            break;
        }
    }
}

