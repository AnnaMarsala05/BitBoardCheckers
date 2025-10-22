#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "game.h"
#include "bitboard.h"  

#ifndef RANK_1
#define RANK_1 0x00000000000000FFULL
#endif
#ifndef RANK_8
#define RANK_8 0xFF00000000000000ULL
#endif


static const uint64_t START_P1 = 0x00000000000055AAULL;  // rows 1..2
static const uint64_t START_P2 = 0x55AA000000000000ULL;  // rows 6..8


static inline uint64_t occ_all(const Game *g) { return g->p1 | g->p1k | g->p2 | g->p2k; }
static inline int on_board(int idx)          { return idx >= 0 && idx < 64; }
static inline int dark_square(int idx)       { int r=idx/8, c=idx%8; return ((r+c)&1)==1; }

static inline int same_color_piece(const Game *g, int turn, int idx){
    uint64_t bit = 1ULL << idx;
    return (turn==1) ? ((g->p1|g->p1k) & bit) != 0 : ((g->p2|g->p2k) & bit) != 0;
}
static inline int is_king(const Game *g, int turn, int idx){
    uint64_t bit = 1ULL << idx;
    return (turn==1) ? ((g->p1k & bit) != 0) : ((g->p2k & bit) != 0);
}
static inline int enemy_at(const Game *g, int turn, int idx){
    uint64_t bit = 1ULL << idx;
    return (turn==1) ? (((g->p2|g->p2k) & bit) != 0) : (((g->p1|g->p1k) & bit) != 0);
}


void game_init(Game *g){
    g->p1  = START_P1;
    g->p1k = 0;
    g->p2  = START_P2;
    g->p2k = 0;
    g->turn = 1;
}

void game_print(const Game *g){
    printf("   a b c d e f g h\n");
    for(int r=0; r<8; ++r){
        printf("%d |", r+1);
        for(int c=0; c<8; ++c){
            int idx = r*8 + c;
            uint64_t bit = 1ULL << idx;
            char ch = dark_square(idx) ? '.' : ' ';
            if (g->p1  & bit) ch = 'r';
            if (g->p1k & bit) ch = 'R';
            if (g->p2  & bit) ch = 'b';
            if (g->p2k & bit) ch = 'B';
            printf("%c", ch);
            if (c < 7) printf(" ");
        }
        printf("| %d\n", r+1);
    }
    printf("   a b c d e f g h\n");
    printf("Turn: Player %d\n", g->turn);
}

int game_has_winner(const Game *g){
    unsigned p1count = __builtin_popcountll(g->p1 | g->p1k);
    unsigned p2count = __builtin_popcountll(g->p2 | g->p2k);
    if (p1count == 0) return 2;
    if (p2count == 0) return 1;
    return 0;
}


static int parse_move(const char *s, int *from, int *to){
    // Accepts "b6-c5" or "b6c5"
    char a[3]={0}, b[3]={0};
    int i=0, j=0, k=0;
    while(s[i] && isspace((unsigned char)s[i])) ++i;
    if(!s[i]) return 0;

    // first square
    if(!isalpha((unsigned char)s[i])) return 0;
    a[j++] = s[i++];
    if(!isdigit((unsigned char)s[i])) return 0;
    a[j++] = s[i++];

    // skip separators
    while(s[i] && !isalpha((unsigned char)s[i])) ++i;
    if(!s[i]) return 0;

    // second square
    if(!isalpha((unsigned char)s[i])) return 0;
    b[k++] = s[i++];
    if(!isdigit((unsigned char)s[i])) return 0;
    b[k++] = s[i++];

    *from = square_to_index(a);
    *to   = square_to_index(b);
    return (*from >= 0 && *to >= 0);
}

static int legal_step_or_capture(Game *g, int from, int to){
    if(!on_board(from)||!on_board(to)) return 0;
            printf("1\n");

    if(!dark_square(from)||!dark_square(to)) return 0;
            printf("2\n");


    uint64_t toBit = 1ULL << to;
    if(occ_all(g) & toBit) return 0; // destination must be empty

            printf("3\n");
    if(!same_color_piece(g, g->turn, from)) return 0;
            printf("4\n");


    int dr = (to/8) - (from/8);
    int dc = (to%8) - (from%8);
    int king = is_king(g, g->turn, from);

    // simple step
    if((dr==1||dr==-1) && (dc==1||dc==-1)){
        if(king) return 1;
        if(g->turn==1 && dr== 1) return 1; // P1 forward
        if(g->turn==2 && dr==-1) return 1; // P2 forward
        return 0;
    }

    // capture
    if((dr==2||dr==-2) && (dc==2||dc==-2)){
        int mid = from + (dr/2)*8 + (dc/2);
        if(!on_board(mid)) return 0;
        if(!enemy_at(g, g->turn, mid)) return 0;
        if(king) return 2;
        if(g->turn==1 && dr== 2) return 2;
        if(g->turn==2 && dr==-2) return 2;
    }
    return 0;
}

static void do_move(Game *g, int from, int to, int capture){
    uint64_t fromBit = 1ULL<<from, toBit = 1ULL<<to;

    if(g->turn==1){
        if(g->p1k & fromBit){ g->p1k ^= fromBit; g->p1k |= toBit; }
        else                { g->p1  ^= fromBit; g->p1  |= toBit; }
    } else {
        if(g->p2k & fromBit){ g->p2k ^= fromBit; g->p2k |= toBit; }
        else                { g->p2  ^= fromBit; g->p2  |= toBit; }
    }

    if(capture){
        int mid = from + ((to-from)/2);
        uint64_t midBit = 1ULL<<mid;
        if(g->turn==1){ if(g->p2k&midBit) g->p2k^=midBit; else g->p2^=midBit; }
        else          { if(g->p1k&midBit) g->p1k^=midBit; else g->p1^=midBit; }
    }

    // promotions
    uint64_t p1_prom = g->p1 & RANK_8; if(p1_prom){ g->p1 ^= p1_prom; g->p1k |= p1_prom; }
    uint64_t p2_prom = g->p2 & RANK_1; if(p2_prom){ g->p2 ^= p2_prom; g->p2k |= p2_prom; }
}

int game_try_move(Game *g, const char *move_str){
    int from, to;
    if(!parse_move(move_str, &from, &to)) return 0;
    int kind = legal_step_or_capture(g, from, to);
            printf("legal step? %d \n",kind );

    if(!kind) return 0;
    do_move(g, from, to, kind==2);
    g->turn = 3 - g->turn; // swap turn
    return 1;
}
