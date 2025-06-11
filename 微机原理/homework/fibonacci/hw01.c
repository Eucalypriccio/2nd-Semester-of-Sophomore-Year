#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    unsigned char *hexNumbers = (unsigned char *)malloc(8);
    
    hexNumbers[0] = 0xcd;
    hexNumbers[1] = 0xcc;
    hexNumbers[2] = 0xcc;
    hexNumbers[3] = 0xcc;
    hexNumbers[4] = 0xcc;
    hexNumbers[5] = 0xcc;
    hexNumbers[6] = 0xf4;
    hexNumbers[7] = 0xbf;

    short int* sint = (short int*)hexNumbers;
    unsigned short int* usint = (unsigned short int*)hexNumbers;
    char* ct = (signed char*)hexNumbers;
    unsigned char* uct = (unsigned char*)hexNumbers;
    int* cint = (int*)hexNumbers;
    unsigned  int* ucint = (unsigned int*)hexNumbers;
    float* fint = (float*)hexNumbers;
    double* dint = (double*)hexNumbers;

    printf("Decimal values (little-endian):\n");

    printf("short int: %hd\n", *(sint));
    printf("unsigned short int: %hu\n", *(usint));
    printf("char: %d\n", *(ct));
    printf("unsigned char: %u\n", *(uct));
    printf("int: %d\n", *(cint));
    printf("unsigned int: %u\n", *(ucint));
    printf("float: %f\n", *(fint));
    printf("double: %lf\n", *(dint));

    free(hexNumbers);
    return 0;
}