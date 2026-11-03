#include "main.h"
/**
 * puts_half - prints half of a string, followed by a new line
 * @str: the string to print
 *
 * Return: void
 */
void puts_half(char *str)
{
	int len = 0;
	int i;

	while (str[len] != '\0')
	{
		len++;
	}

	if (len % 2 == 0)
	{
		for (i = len / 2; str[i] != '\0'; i++)
		{
			_putchar(str[i]);
		}
	}
	else
	{
		for (i = (len - 1) / 2 + 1; str[i] != '\0'; i++)
		{
			_putchar(str[i]);
		}
	}

	_putchar('\n');
}

