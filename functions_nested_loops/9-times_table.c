#include "main.h"

/**
 * times_table - prints the 9 times table, starting with 0
 */
void times_table(void) {
  int r, c, prod;

  for (r = 0; r <= 9; r++) {
    for (c = 0; c <= 9; c++) {
      prod = r * c;
      if (c == 0) {
        _putchar('0');
      } else {
        _putchar(',');
        _putchar(' ');
        if (prod >= 10) {
          _putchar((prod / 10) + '0');
          _putchar((prod % 10) + '0');
        } else {
          _putchar(' ');
          _putchar(prod + '0');
        }
      }
    }
    _putchar('\n');
  }
}
