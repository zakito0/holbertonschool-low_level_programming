#include "main.h"
/**
 * _strncat - concatenates two strings,
 * @dest: destination string
 * @src: source string
 * @n: number of bytes to concatenate
 * Return: pointer to the destination string
 */
char *_strncat(char *dest, char *src, int n)
{
    char *ret = dest;
    int i, j;

    for (i = 0; *dest; i++)
        dest++;
    for (j = 0; j < n && src[j]; j++)
        dest[i + j] = src[j];
    dest[i + j] = '\0';
    return (ret);
}