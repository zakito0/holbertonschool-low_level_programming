#include "main.h"
#include <stdio.h>

char *_memcpy(char *dest, char *src, unsigned int n)
{
    printf("%s", _memcpy(dest, src, 5));
    printf(n);
    return(0);
}
