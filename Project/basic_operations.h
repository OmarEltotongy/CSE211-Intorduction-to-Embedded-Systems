#ifndef BASIC_OPERATION_H
#define BASIC_OPERATION_H

void swap(char *x, char *y);
// Function to reverse `buffer[i…j]`
char* reverse(char *buffer, int i, int j);
// Iterative function to implement `itoa()` function in C
char* itoa(int value, char* buffer, int base);

#endif