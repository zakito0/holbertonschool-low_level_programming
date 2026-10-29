#include "main.h"

/**
 * print_number - prints an integer using _putchar
 * @n: integer to print
 */
void print_number(int n)
{
	unsigned int num;

	/* handle negative numbers */
	if (n < 0)
	{
		_putchar('-');
		num = -n;
	}
	else
	{
		num = n;
	}

	/* recursive call to print left digits */
	if (num / 10)
		print_number(num / 10);

	/* print last digit */
	_putchar((num % 10) + '0');
}

