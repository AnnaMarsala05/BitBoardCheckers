
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "bitboard.h"


int square_to_index(const char *coord){
// coord like "a1".."h8" (top row is 1 (row 0), bottom is 8 (row 7))
if(!coord||strlen(coord)<2) return -1;
char f = tolower((unsigned char)coord[0]);
char r = coord[1];
if(f<'a'||f>'h'||r<'1'||r>'8') return -1;
int col = f - 'a';
int row = (r - '1'); // row 0 is top
return row*8 + col; // bit index
}


void index_to_square(int idx, char out[3]){
if(idx<0||idx>63){ out[0]=out[1]='?'; out[2]='\0'; return; }
int row = idx/8, col = idx%8;
out[0] = 'a'+col; out[1] = '1'+row; out[2]='\0';
}