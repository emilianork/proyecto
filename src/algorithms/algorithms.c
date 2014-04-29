#include "algorithms/algorithms.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <string.h>

#define STR_LENGTH 7

void rand_str(char *dest, int length)
{
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
		rand();
        int index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}
