#include <stdio.h>

/**
 * - Fixed the argv declaration to char *argv[]
 * - Replaced the no-op argv[1] with a printf statement using argv[i]
 * - Added a standard return 0 at the end
 */
int main(int argc, char *argv[])
{
    printf("My name is %s\n", argv[1]);
    return 0;
}