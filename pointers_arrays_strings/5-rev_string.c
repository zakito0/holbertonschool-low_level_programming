#include "main.h"
/*
*rev_string  - reverses a string
*
*Return: Return a reverse string
*/
void rev_string(char *s)
{
int n = 0;
int m = 0;

    char temp;

    while (s[n] != '\0')
    {
        n++;
    }
    n--;
    while (m < n)
    {
        temp = s[m];
        s[m] = s[n];
        s[n] = temp;
        m++;
        n--;
    }
}