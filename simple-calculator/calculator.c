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
    }
    
    return (0);
}