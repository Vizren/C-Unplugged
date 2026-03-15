#include <stdio.h>
#include <string.h>
#include "log.h"

void mylog(const char* cmdline) {
    FILE* f = fopen("command_history.txt", "a");
    if (!f){
        return;
    }
    fprintf(f, "%s\n", cmdline);
    fclose(f);
}

void showhistory(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f){
        printf("(no history)\n");
        return;
    }
   char line[256];
    while (fgets(line, sizeof(line), f)) {
        line[strcspn(line, "\n")] = '\0';

        int cmd;
        if (sscanf(line, "%d", &cmd) != 1) {
            printf("  Unknown Command - %s\n", line);     // fallback
            continue;
        }

        switch (cmd) {
            case 0:  printf("   0 - help\n"); break;
            case 1:  printf("   1 - list songs\n"); break;
            case 2:  printf("   2 - playlist add-song\n"); break;
            case 3:  printf("   3 - list playlists\n"); break;
            case 4:  printf("   4 - playlist create\n"); break;
            case 5:  printf("   5 - playlist view\n"); break;
            case 6:  printf("   6 - playlist remove-song\n"); break;
            case 7:  printf("   7 - queue add song\n"); break;
            case 8:  printf("   8 - queue add playlist\n"); break;
            case 9:  printf("   9 - queue show\n"); break;
            case 10: printf("  10 - play\n"); break;
            case 11: printf("  11 - next\n"); break;
            case 12: printf("  12 - prev\n"); break;
            case 13: printf("  13 - queue remove song\n"); break;
            case 14: printf("  14 - history\n"); break;
            case 15: printf("  15 - exit\n"); break;
            default: printf("  %d - Unknown\n", cmd);
        }
    }
    fclose(f);
}
