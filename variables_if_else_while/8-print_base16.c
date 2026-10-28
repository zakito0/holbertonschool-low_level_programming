#include <stdio.h>
/**
 * main - Prints numbers in base 16 *
 * Return: Always 0
 */
int main(void)
{
	char base16;
    char base16L;

	for (base16 = '0'; base16 <= '9'; base16++)
	{
		putchar(base16);
	}
    for (base16L = 'a'; base16L <= 'f'; base16L++)
    {
        putchar(base16L);
    }
	putchar('\n');
	return (0);
}