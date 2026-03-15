#include <stdio.h>
#include <stdlib.h>
#include "song.h"
#include "playlist.h"
#include "queue.h"
#include "storage.h"
#include "cli.h"

int main(void) {
    printf("\n\nC-Unplugged starting...\n Type '0' for Command Menu\n\n");

    song* library = loadsongs("songlib.txt");

    playlist* playlists = loadplaylists("playlists.txt");

    queue* q = queuecrt();

    cli_run(&library, &playlists, q);

    if (saveplaylists("playlists.txt", playlists) == 0){
        printf("Playlists saved to playlists.txt\n\n");
    } else{
        printf("Failed to save playlists.txt\n");
    }

    songfree(library);
    playlistfree(playlists);
    queuefree(q);

    printf("Goodbye :)\n\n");
    return 0;
}
