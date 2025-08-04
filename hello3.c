/*  hello3.c
    This program, corresponding to chapter 10 in the book, does file 
    manipulation. Added fgeatures will be entering files names
    through program parameters.
*/

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
  int param = 0;
  printf("Program parameters (strings) at run time:\n");
  while (param < argc)
  {
    printf("Parameter %d is %s\n", param, argv[param]);
    param++;
  }
 
 
 
 
  return 0;
}