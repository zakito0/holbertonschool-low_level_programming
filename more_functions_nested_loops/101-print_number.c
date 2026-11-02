#include "main.h"

/**
 * print_number - prints an integer
 * @n: integer to be printed
 */
void print_number(int n)
{
	unsigned int num, m, count;

	if (n < 0)
	{
		_putchar('-');
		num = -((unsigned int)n);
	}
	else
	{
		num = n;
	}

	m = num;
	count = 1;

	while (m > 9)
	{
		m /= 10;
		count *= 10;
	}

	while (count >= 1)
	{
		_putchar((num / count) + '0');
		num %= count;
		count /= 10;
	}
}

