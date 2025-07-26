/* A basic C language statement review exercise
    This ptogram will change A LOT with learning steps

*/

// test some includes
/* MariaDB include */
#include <mariadb/mysql.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main()
{
  int a = 1;
  int x = 0;
  char character;
  char cword[10] = "\0\0\0\0\0\0\0\0\0\0";

  printf("Hello world!");
  
  // test some character moves and print statements
  x = 0;
   
  printf("\n\nStart: 'x' variable, start: %i ", x);
  printf("String length: %li, string: %s \n", strlen(cword), cword);
  x++;
  character = 'w';
  cword[0] = character;
  printf(" Next: 'x': %i, char: %c, word: %s, length: %li\n", x, character, cword, strlen(cword));
  x++;
  character = 'o';
  cword[1] = character;
  printf(" Next: 'x': %i, char: %c, word: %s, length: %li\n", x, character, cword, strlen(cword));
  x++;
  character = 'r';
  cword[2] = character;
  printf(" Next: 'x': %i, char: %c, word: %s, length: %li\n", x, character, cword, strlen(cword));
  x++;
  character = 'd';
  cword[3] = character;
  printf(" Next: 'x': %i, char: %c, word: %s, length: %li\n", x, character, cword, strlen(cword));
  x++;
  character = 's';
  cword[4] = character;
  printf(" Added character for good luck: %c\n", character);
  printf(" Result: 'x': %i, char: %c, word: %s, length: %li\n", x, character, cword, strlen(cword));

  // while loop test - test before the loop iteration
  printf("-------------------------\n");
  a = 0;
  printf("while: setup - a is %i, x is %i\n", a, x);
  while ( a <= x)
  {
    if (a == 0)
      printf("while:  A is 0: -> %i\n", a);
    else  
      printf("while:  A is %i <= %i\n", a, x);
    a++;
  } 

  // do loop test - test after the loop interation
  printf("-------------------------\n");
  a = 0;
  printf("do: setup - a is %i, x is %i\n", a, x);
  do 
  {
    if (a == 0)
      printf("do: A is 0: -> %i\n", a);
    else  
      printf("do: A is %i <= %i\n", a, x);
    a++;
  } while (a <= x);


  // for-loop test  - initial value, test before iteration, increment after iteration
  printf("-------------------------\n");

  printf("for: setup - x is %i and a is iterated 0 through %i\n", x, x );
  for (a =0; a <=x; a++)
  {
    printf("for : Iteration: a: %i\n", a);
  
    //if (a == 0)
    //  printf("for: A is 0: %i\n", a);
    //else  
    //  printf("for: A is not 0 but %i\n", a);
  }

  // For loop to exercise a switch statement
  // not very pretty, but it does the job
  printf("-------------------------\n");
  printf("switch/case: setup - x is %i and a is iterated 0 through %i\n", x, x );
  for (a =0; a <=x; a++)
  {
    printf("loop: a is %i: ", a);
    switch (a)
    {
        case 0: 
          {
            printf(" Case 0: ");
            if (a == x) printf(" equals x: %i", x);
            else        printf(" not equal to x: %i", x);
            break;
        }
        case 1: 
          {
           printf(" Case 1: ");
           if (a == x)  printf(" equals x: %i", x);
           else         printf(" not equal to x: %i", x);
           break;
          }
        case 2: 
          {
           printf(" Case 2: ");
           if (a == x)  printf(" equals x: %i", x);
           else         printf(" not equal to x: %i", x);
           break;
          }
        case 3: 
          {
           printf(" Case 3: ");
           if (a == x)  printf(" equals x: %i", x);
           else         printf(" not equal to x: %i", x);
           break;
          }
        case 4: 
          {
           printf(" Case 4 :");
           if (a == x)  printf(" equals x: %i", x);
           else         printf(" not equal to x: %i", x);
           break;
          }
        case 5: 
          {
           printf(" Case 5: ");
           if (a == x)  printf(" equals x: %i", x);
           else         printf(" not equal to x: %i", x);
           break;
          }
        case 6: 
          {
           printf(" Case 6: ");
            if (a == x)  printf(" equals x: %i", x);
           else         printf(" not equal to x: %i", x);
           break;
          }
        case 7: 
          {
           printf(" Case 7: ");
           if (a == x)  printf(" equals x: %i", x);
           else         printf(" not equal to x: %i", x);
           break;
          }
        default: 
          {
           printf(" *Default*: ");
           if (a == x)  printf(" equals x: %i", x);
           else         printf(" Big number but not equal to x: %i", x);
           break;
          }
    }
    printf("\n");
  }



  return 0;
}
