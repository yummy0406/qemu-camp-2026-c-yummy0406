#include <stdint.h>
#include <stdio.h>

uint32_t swap_endian(uint32_t num) {
    uint32_t result;
    __asm__ volatile("bswap %0" : "=r"(result) : "0"(num));
    return result;
}

int main(int argc, char* argv[]) {
    uint32_t num = 0x78563412;
    uint32_t swapped = swap_endian(num);
    printf("0x%08x -> 0x%08x\n", num, swapped);
    return 0;
}