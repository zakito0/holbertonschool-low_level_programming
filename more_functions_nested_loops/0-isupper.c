#include "main.h"
#include <stdio.h>
/**
 * _isupper - see if a character is uppercase
 * @c: the character to verify
 *
 * Return: 1 if the character is uppercase, else 0 if it is not
     */
int _isupper(int c)
{
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

int main(void)
{
    int c;

    c = 'c';
    printf("%d\n", _isupper(c));
    return (0);
}