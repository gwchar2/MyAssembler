#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define BITS_IN_INT 12

void int_to_12bit(int32_t integer, char *binary_str) {
    // Ensure the integer is within the valid range for 12-bit representation
    if (integer < -(1 << (BITS_IN_INT - 1)) || integer >= (1 << (BITS_IN_INT - 1))) {
        printf("Integer value is out of range for 12 bits\n");
        return;
    }
     printf("%d\n", -(1 << (BITS_IN_INT - 1))) ;
     printf("%d\n", (1 << (BITS_IN_INT - 1))) ;

    // Handle negative numbers using Two's complement
    if (integer < 0) {
        integer += (1 << BITS_IN_INT); 
        printf( "%d\n", integer);
        printf("1 << BITS_IN_INT: %d\n", 1 << BITS_IN_INT);

    }

    // Store the binary representation of the integer in the string
    for (int i = BITS_IN_INT - 1; i >= 0; i--) {
        int bit = (integer >> i) & 1;
        *binary_str++ = bit + '0'; // Convert the bit to ASCII '0' or '1'
    }
    *binary_str = '\0'; // Null terminate the string
}

int main() {
    int32_t input_int = 3;
    input_int = input_int << 5;
    char binary_str[BITS_IN_INT + 1]; // +1 for null terminator

    printf("Input Integer: %d\n", input_int);
    int_to_12bit(input_int, binary_str);
    printf("12-bit Binary: %s\n", binary_str);

    return 0;
}
