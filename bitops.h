#ifndef BITOPS_H
#define BITOPS_H
#include <stdio.h>
#include <stdint.h>
static inline uint32_t set_bit32(uint32_t value, int position) {
    return (position >=0 && position <32)? value | (1u<<position) : value;
}
static inline uint32_t clear_bit32(uint32_t value, int position) {
    return (position >=0 && position <32)? value & ~(1u<<position) : value;
}
static inline uint32_t toggle_bit32(uint32_t value, int position) {
    return (position >=0 && position <32)? value ^ (1u<<position) : value;
}
static inline int get_bit32(uint32_t value, int position){ 
    return (position>=0 && position <32)? ((value>>position) &1u):0;
}

static inline int count_bits32(uint32_t value){
    int c=0; while(value){ value &=value-1; ++c;} return c;
}
static inline uint32_t shl32(uint32_t value, int k){
     return (k>=0 && k<32)? value<<k : 0u; 
}
static inline uint32_t shr32(uint32_t value, int k){ 
    return (k>=0 && k<32)? value>>k : 0u; 
}


void print_binary32(uint32_t value);
void print_hex32(uint32_t v);
#endif // BITOPS_H