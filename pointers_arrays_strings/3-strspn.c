#include "main.h"
#include <stdio.h>
/**
 * _strspn - Gets the length of a prefix substring.
 * @s: The string to be scanned.
 * @accept: The string containing the characters to match.
 * 
 * Return: The number of bytes in the initial segment of s
 * which consist only of bytes from accept.
 */
unsigned int _strspn(char *s, char *accept)
{
    int i, len, j;
    len = 0;
    for (i = 0; s[i] != '\0'; i++)
    {
        for (j = 0; accept[j] != '\0'; j++)
            {
                if (s[i] == accept[j])
                    {
                        len++;
                        break;

                    }
            }
            if (s[i] != accept[j])
            {
                return (len);
            }
    }
    return (len);
}