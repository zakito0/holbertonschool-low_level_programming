#include <stdio.h>

/**
 * main - finds and prints the largest prime factor of 612852475143
 *
 * Return: Always 0
 */
int main(void)
{
	long n = 612852475143L;
	long div = 2;

	/* divide out factors until 1 is left */
	while (n > 1)
	{
		if (n % div == 0)
			n /= div;
		else
			div++;
	}
	printf("%ld\n", div);

	return (0);
}
