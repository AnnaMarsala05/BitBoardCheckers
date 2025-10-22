#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "game.h"

static void print_help(void) {
    puts("Commands:");
    puts("  b6-c5      make a simple move");
    puts("  c3-e5      make a capture move");
    puts("  h or help  show this help");
    puts("  q          quit");
}

int main(void) {
    Game g;
    game_init(&g);

    puts("BitBoard Checkers - Playable Demo");
    print_help();
    puts("");

    char buf[128];
    while (1) {
        game_print(&g);

        int w = game_has_winner(&g);
        if (w) {
            printf("Player %d wins!\n", w);
            break;
        }

        printf("P%d> ", g.turn);
        if (!fgets(buf, sizeof buf, stdin)) break;

        // trim newline
        size_t n = strlen(buf);
        if (n && (buf[n-1] == '\n' || buf[n-1] == '\r')) buf[--n] = '\0';
        if (n == 0) continue;

        // commands
        if (buf[0]=='q' || buf[0]=='Q') break;
        if (!strcmp(buf,"h") || !strcmp(buf,"help") || !strcmp(buf,"?")) {
            print_help();
            continue;
        }

        // try to apply a move like "b6-c5" or "b6c5"
        if (!game_try_move(&g, buf)) {
            puts("Illegal move. Try again (hint: b6-c5).");
        }
    }

    return 0;
}