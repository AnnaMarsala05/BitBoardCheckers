#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdint.h>

// Directions (relative shifts)
#define DIR_NE (-7)
#define DIR_NW (-9)
#define DIR_SE (+9)
#define DIR_SW (+7)


// Utility
int square_to_index(const char *coord); // e.g., "b6" -> index
void index_to_square(int idx, char out[3]);


#endif // BITBOARD_H


