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