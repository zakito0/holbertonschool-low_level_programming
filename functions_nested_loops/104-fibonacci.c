#include <stdio.h>

/**
 * main - prints the first 98 Fibonacci numbers
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
	int count;
	unsigned long f1_head = 0, f1_tail = 1;
	unsigned long f2_head = 0, f2_tail = 2;
	unsigned long next_head, next_tail;
	unsigned long base = 1000000000UL;

	printf("%lu, %lu", f1_tail, f2_tail);
	for (count = 3; count <= 98; count++)
	{
		next_tail = f1_tail + f2_tail;
		next_head = f1_head + f2_head + (next_tail / base);
		next_tail %= base;

		if (next_head > 0)
			printf(", %lu%09lu", next_head, next_tail);
		else
			printf(", %lu", next_tail);

		f1_head = f2_head;
		f1_tail = f2_tail;
		f2_head = next_head;
		f2_tail = next_tail;
	}
	printf("\n");

	return (0);
}
