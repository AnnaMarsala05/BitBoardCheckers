#include "bitops.h"


void print_binary32(uint32_t v){
for(int i=31;i>=0;--i){ putchar( (v>>i)&1u ? '1':'0'); if(i%4==0 && i) putchar(' '); }
putchar('\n');
}


void print_hex32(uint32_t v){ printf("0x%08X\n", v); }