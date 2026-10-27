#include <stdio.h>

int main(void)
{
    char c;

    /* 1. Afficher les minuscules de 'a' à 'z' */
    c = 'a';
    while (c <= 'z')
   {
        putchar(c); /* 1ère utilisation de putchar */
        c++;
    }

    /* 2. Afficher les majuscules de 'A' à 'Z' */
    c = 'A';
    while (c <= 'Z')
    {
        putchar(c); /* 2ème utilisation de putchar */
        c++;
    }

    /* 3. Afficher le saut de ligne */
    putchar('\n'); /* 3ème utilisation de putchar */

    return (0);
}
