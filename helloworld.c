/* A basic C language statement review exercise
    This ptogram will change A LOT with learning steps

*/

// test some includes
/* MariaDB include */
// #include <mariadb/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function declaration. Note that the parms are only type declarations here
// You may include var names, but those are unneeded here
int print_examples (int *, char []);
int comp_strings(long int *, char [], char []);


//main
// All C programs must have a 'main'. It always begins here
int main()
{
  int a = 1;
  int x = 0;
  long int strlength = 0;
  char character;
  char cword[10] = "\0\0\0\0\0\0\0\0\0\0";

//  Normally this declaration is found here, but I'm putting it where it will be used
//  int *ptr_to_a = NULL;

  printf("Hello world!");
  
  // test some character moves and print statements
  x = 0;
   
  printf("\n\nStart: 'x' variable, start: %i ", x);
  printf("String length: %li, string: %s \n", strlen(cword), cword);
  x++;
  character = 'w';
  cword[0] = character;
  printf(" Next: 'x': %i, char: %c, word: %s, length: %ld\n", x, character, cword, strlen(cword));
  x++;
  character = 'o';
  cword[1] = character;
  printf(" Next: 'x': %i, char: %c, word: %s, length: %ld\n", x, character, cword, strlen(cword));
  x++;
  character = 'r';
  cword[2] = character;
  printf(" Next: 'x': %i, char: %c, word: %s, length: %ld\n", x, character, cword, strlen(cword));
  x++;
  character = 'd';
  cword[3] = character;
  printf(" Next: 'x': %i, char: %c, word: %s, length: %ld\n", x, character, cword, strlen(cword));
  x++;
  character = 's';
  cword[4] = character;
  printf(" Added character for good luck: %c\n", character);
  printf(" Result: 'x': %i, char: %c, word: %s, length: %ld\n", x, character, cword, strlen(cword));

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
  printf("-------------------------\n");

  printf("Some pointer stuff:\n");

  // Normally this is found with the other declarations, but for code clarity
  // I'm putting it here. This is all a coding refresher, after all

  int *ptr_to_a = NULL;       // declare the pointer, default it to NULL
  ptr_to_a = &a;              // point it to the var you intend
  a = 5;                      // give the var a value to work with
  printf("The starting value of a is %d\n", a);    // display it at the start

  *ptr_to_a = 6;              // change the var's value via the pointer. note the '*' prefix
  printf("The updated value of a, updated through the pointer, is %d\n", a);    // display the result

  // display the various parts of pointer usage
  // Note this example converts a pointer to an printable integer value.
  // Conversion/cast is needed to compile this without compiler warning(s)
  printf("The value of physical pointer (physical address) ptr_to_a is %ld \n",( (long int)ptr_to_a) );  // display the value of the actual pointer (a physical address)
  printf("The value of a, using the pointer, is %d\n", *ptr_to_a);      // display the value of the address/var the pointer is pointing to
  printf("The physical address of a is %ld\n", ((long int) &a) );             // display the physical address of the 'a' variable

  printf("-------------------------\n");
  printf("Some function stuff with strings:\n");
  printf("Initial (pre-function call) value of a: %d\n", a);
  print_examples (&a, "This is a test string");
  printf("main: updated value of \'a\' (ie. calculated string length): %d\n",a);
  printf("-------------------------\n");
  
  // Some string manipulation stuff
  comp_strings(&strlength, "This is a first string.", "This is another string.");
//  comp_strings(&strlength, "This is a first string", "This is a second string");
  printf("Combined length of the two strings: %ld\n", strlength);



printf("\n");
  return 0;
} /* main */

// function call 
// This is somewhere past 'main', where I like it (main should be at the top)
// To avoid compiler error/warning, there is a function declaration at the top, so the 
// the compiler will know what the function call should look like when it encounters one.
int print_examples (int * l, char b[])
{
  int charlen = 0;

  charlen = strlen(b);
  *l = charlen;  
  
  printf("Print_examples: parms passed: a (int) = %d, string (char string): \"%s\", length: %d.\n", *l, b, charlen);

  return 0;
} /* print_examples */

int comp_strings(long int *l, char str1[], char str2[])
{
  // do some string manipulations as outlined in the book
  // sometimes it's worth initializing a string field but usually it's just extra work
  //  char full_text[50] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";
  char full_text[50];

  printf ("Comp: Value passed by \'a\': %ld\n", *l );
  printf("String 1 passed by \'str1\': %s, length: %ld\n", str1, strlen(str1));
  printf("String 2 passed by \'str2\': %s, length: %ld\n", str2, strlen(str2));
  *l = strlen(str1) + strlen(str2);
  if ( strlen(str1) == strlen(str2))
    printf("Strings are the same length.\n");
  else 
    printf("Strings are not the same length.\n");
  if ( strcmp(str1, str2) == 0 )
    printf("Strings are the identical.\n");
  else
    printf("Strings are diffewrent.\n");
  strcpy(full_text, str1);
  printf("Copying \'str1\' into the full-text space: \'%s\'. \n", full_text);
  strcat(full_text, " ");
  strcat(full_text, str2);
  printf("Adding a space and then \'str2\' into the full-text space: \'%s\'. String length: %ld\n", full_text, strlen(full_text));
 
  return 0;
}