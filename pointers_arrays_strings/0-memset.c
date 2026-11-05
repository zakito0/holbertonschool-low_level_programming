#include "main.h"
#include <stdio.h>
/**
 * _memset - a Function that ...
 * @s: Description of s.

 * @b: Description of b.
 * @n: Description of n.
 * Return: Description of the return value.
 */
char *_memset(char *s, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
	{
		s[i] = b;
	}
	return (s);
}

