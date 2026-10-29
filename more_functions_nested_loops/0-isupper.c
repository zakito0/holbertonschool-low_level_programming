#include "main.h"
#include <stdio.h>
/**
 * _isupper - checks for uppercase character
 * @c: The character to check
 *
 * Return: 1 if c is uppercase, 0 otherwise
 */
int _isupper(int c)
{
    if (c >= 'A' && c <= 'Z')
    {
        return (1);
    }
    else
    {
        return (0);
    }
}

int main(void)
{
    int c;

    for (c = 0; c <= 127; c++)
    {
        if (_isupper(c))
        {
            printf("%c is uppercase\n", c);
        }
        else
        {
            printf("%c is not uppercase\n", c);
        }
    }

    return (0);
}