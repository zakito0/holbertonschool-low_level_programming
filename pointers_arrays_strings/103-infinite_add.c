#include "main.h"

/**
 * infinite_add - adds two numbers
 * @n1: first number string
 * @n2: second number string
 * @r: buffer for result
 * @size_r: buffer size
 *
 * Return: pointer to r or 0
 */
char *infinite_add(char *n1, char *n2, char *r, int size_r)
{
	int l1, l2, max, sum, carry = 0;
	int i, j, k;
	char tmp;

	for (l1 = 0; n1[l1] != '\0'; l1++)
		;
	for (l2 = 0; n2[l2] != '\0'; l2++)
		;

	max = (l1 > l2) ? l1 : l2;
	if (max + 1 > size_r)
		return (0);

	k = 0;
	for (i = l1 - 1, j = l2 - 1; i >= 0 || j >= 0 || carry; i--, j--, k++)
	{
		if (k >= size_r - 1)
			return (0);
		sum = carry;
		if (i >= 0)
			sum += n1[i] - '0';
		if (j >= 0)
			sum += n2[j] - '0';
		carry = sum / 10;
		r[k] = (sum % 10) + '0';
	}
	r[k] = '\0';

	for (i = 0, j = k - 1; i < j; i++, j--)
	{
		tmp = r[i];
		r[i] = r[j];
		r[j] = tmp;
	}
	return (r);
}

