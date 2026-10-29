#include "main.h"

/**
 * print_diagonal - draws a diagonal line in terminal
 * @n: number of times to print \
 */
void print_diagonal(int n)
{
	int i;
	int j;

	/* only print if n > 0 */
	if (n > 0)
	{
		for (i = 0; i < n; i++)
		{
			/* print spaces before \ */
			for (j = 0; j < i; j++)
				_putchar(' ');
			_putchar('\\');
			_putchar('\n');
		}
	}
	else
	{
		_putchar('\n');
	}
}

