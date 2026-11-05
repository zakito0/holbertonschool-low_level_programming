#include <stdio.h>
#include <unistd.h>

/**
 * main - Entry point
 *
 * Description: Displays a menu for the Calculito Calculator
 * and waits for the user to press 0 key to quit the program.
 *
 * Return: Always 0 (Success)
 */
int main(void)
{
    int c;
    int A;
    int B;
    int result;
    c = 0;

    puts("Welcome to Calculito !");
    puts("1) Add");
    puts("2) Substract");
    puts("3) Multiply");
    puts("4) divide");
    puts("0) Quit");

    while (c != '0')
    {
        c = getchar();
        if (c != '0' && c != '\n')
        {
        printf("Invalid Choice\n");
        }
        if (c == '1')
        {
          printf("Enter a number: ");
          scanf("%d", &A);
          printf("Enter a second number: ");
          scanf("%d", &B);
          result = A + B;
          printf("Result: %d", result);
        }
        if (c == '2')
        {
          printf("Enter a number: ");
          scanf("%d", &A);
          printf("Enter a second number: ");
          scanf("%d", &B);
          result = A - B;
          printf("Result: %d", result);
        }
        if (c == '3')
        {
          printf("Enter a number: ");
          scanf("%d", &A);
          printf("Enter a second number: ");
          scanf("%d", &B);
          result = A * B;
          printf("Result: %d", result);
        }
        if (c == '4')
        {
          printf("Enter a number: ");
          scanf("%d", &A);
          printf("Enter a second number: ");
          scanf("%d", &B);
          result = A / B;
          printf("Result: %d", result);
        }
    }
    
    return (0);
}