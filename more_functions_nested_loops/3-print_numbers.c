#include "main.h"

/**
 * print_numbers - prints the numbers from 0 to 9
 */
void print_numbers(void)
{
	char c;

	/* loop from 0 to 9 and print each digit */
	for (c = '0'; c <= '9'; c++)
		_putchar(c);
	_putchar('\n');
}

