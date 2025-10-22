#ifndef GAME_H
#define GAME_H
#include <stdint.h>

typedef struct {
    uint64_t p1, p1k;
    uint64_t p2, p2k;
    int turn;           // 1 or 2
} Game;

/* Prototypes */
void game_init(Game *g);
void game_print(const Game *g);
int  game_has_winner(const Game *g);
int  game_try_move(Game *g, const char *move_str);

#endif