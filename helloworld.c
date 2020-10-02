#include <stdio.h>

int x = 0;

int main()
{
    printf("\n\n'x' variable, start: %i \n", x);
    x = 1;
    printf("Hello world!\n");
    printf("'x' variable: %i \n", x);
    printf("Isn't this fun!\n");
    printf("Ammended third line added from master!\n");
    printf("Yet another line for a test.\n");
    x = 2;
    printf("'x' variable: %i \n", x);
    printf("One last line for good luck.\n\n\n");
    return 0;
}
