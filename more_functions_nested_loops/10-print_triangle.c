#include "main.h"

/**
 * print_triangle - prints a right-aligned triangle
 * @size: size of the triangle
 */
void print_triangle(int size)
{
	int i;
	int j;

	/* only print if size > 0 */
	if (size > 0)
	{
		for (i = 1; i <= size; i++)
		{
			/* print spaces first */
			for (j = 0; j < size - i; j++)
				_putchar(' ');

			/* print hashtags */
			for (j = 0; j < i; j++)
				_putchar('#');

			_putchar('\n');
		}
	}
	else
	{
		_putchar('\n');
	}
}

