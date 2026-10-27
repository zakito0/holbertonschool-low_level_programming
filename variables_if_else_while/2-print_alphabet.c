#include <stdio.h>
/* Entry point -  0 
*
* Exit with Success 0 */

int main (void){
    char ch;

    /* Loop from 'a' to 'z' */
    for (ch = 'a'; ch <= 'z'; ch++) {
        printf("%c ", ch);
    }

    printf("\n"); /* New line after printing all letters */
    return 0;
}

