#include <stdio.h>
/**
 * main - Prints numbers in base 16 *
 * Return: Always 0
 */
int main(void)
{
	char base16;

	for (base16 = '0'; base16 >= 'F'; base16--)
	{
		putchar(base16);
	}
	putchar('\n');
	return (0);
}