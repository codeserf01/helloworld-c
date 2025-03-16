/* MariaDB include */
#include <mariadb/mysql.h>

#include <stdio.h>
#include <stdlib.h>

int x = 0;

int main()
{
   
    printf("\n\n'x' variable, start: %i \n", x);
    x = 0;
    printf("%i - Hello world!", x);
    printf(" - 'x' variable: %i \n", x);

    x++;
    printf("%i - Isn't this fun!", x);
    printf(" - 'x' variable: %i \n", x);

    x++;
    printf("%i - Ammended third line added from master!", x);
    printf(" - 'x' variable: %i \n", x);

    x++;
    printf("%i - Yet another line for a test.", x);
    printf("'x' variable: %i \n", x);

    x++;
    printf("%i - One more line for good luck.", x);
    printf(" - End value for x: %i\n\n\n", x);
    return 0;
}
