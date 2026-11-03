#include "main.h"

/**
 * _atoi - converts a string to an integer
 * @s: string to be converted
 *
 * Return: the int converted from the string
 */
int _atoi(char *s)
{
    int i;
    int sign = 1;
    int started = 0;
    unsigned int result = 0;

    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == '-')
        {
            sign *= -1;
        }

        if (s[i] >= '0' && s[i] <= '9')
        {
            started = 1;
            result = (result * 10) + (s[i] - '0');
        }
        else if (started == 1)
        {
            break;
        }
    }

    return (result * sign);
}