#include <stdio.h>

/**
 * main - finds and prints the largest prime factor of 612852475143
 *
 * Return: Always 0
 */
int main(void)
{
	long int n = 612852475143;
	long int p = 2;

	while (p < n)
	{
		if (n % p == 0)
		{
			n = n / p;
			p = 2;
		}
		else
		{
			p++;
		}
	}
	printf("%ld\n", n);

	return (0);
}

