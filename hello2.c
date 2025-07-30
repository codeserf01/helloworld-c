/* Hello2.c
  A C language refesher exercise, this one to try out terminal input processing.
  There are two input methods used initially. The first one is using a 'bulk' call
  'scanf' call, that will read everything into a buffer accuming that the input string
  does not overflow the buffer (very bad!!). The second uses 'fgets' to read in a specific
  number of chars into the buffer potentially limiting the possibility of an overflow
  condition. This is the better option, generally, but sometimes a 'scanf' will do under 
  the right circumstances.
  Note: Use one or the other, NOT BOTH in the same program! 'scanf' may leave behind some extra
  unread input data behind, causing bad results for a following 'fgets' call. 

  The third method is processing the program input parameters. This pgm will only display what 
  was entered as parms at run time.


*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// more learning of C features

int main(int argc, char  * argv[] )
{
    char in_name_str[80]; 
    int  in_num = 0;
    
    printf("Hello2 chiming in...\n");

  // display the program's input parameters
  // Display the full parameter text

    int param = 0;
    printf("Program parameters (strings) at run time:\n");
    while (param < argc)
    {
      printf("Parameter %d is %s\n", param, argv[param]);
      param++;
    }
    // Simple alternative: display the first character of each parameter
    printf("Program parameters - first character of each parm at run time:\n");
    param = 0;
    while (param < argc)
    {
      printf("Parameter %d is %c\n", param, *argv[param]);
      param++;
    }



    // First way: Read typed input directly, whatever it is and whatever size it is
    // There may be a buffer overload using this method (i.e.) more chars are entered than the buffer size allows
    printf("First pass..............\n");
    printf("What is your name? : ");
    scanf("%s", in_name_str);
    // printf("\n");
    // Here it is assumed that a proper number is entered - another mistake.
    printf("What is the number? : ");
    scanf( "%d", &in_num);
    // printf("\n");
    printf("%s, the number you entered is %d. \n", in_name_str, in_num);
    printf("---------------------------\n");
     // Re-do the above but this time reduce any possible garbage and/or input buffer overflow

    // Second step: Put the var definitions here, to help readability of the next few lines
    char in_chars_str[25], in_number_str[25], in_name[25];
    int in_number;

    fgets( in_chars_str, sizeof(in_chars_str), stdin);    // special case: get any leftover input chars to throw away
    // printf("More letters read: %s\n", in_chars_str);   // Debug:  show any remaining chars
    memset(in_chars_str, '\0', sizeof(in_chars_str));     // initialize the input buffer with binary zeros
    printf("Second try, this time different: \nWhat is your name? : ");        // Ask for user name
    fgets( in_chars_str, sizeof(in_chars_str), stdin);    // Read in enough to fill the input buffer
    sscanf( in_chars_str, "%s", in_name);                 // extract the text minus the line feed
    // printf("More letters read: %d, string read in: %s\n", letters_read, in_chars_str);  // Debug: display input
    // printf("((--in_chars_str: %s, in_name_str: %s, Letters_read: %d))\n", in_chars_str, in_name, letters_read); // Debug: show what was entered
    printf("And what is the number? :");                  // Ask user for a number

    // Same read type as above but look for a valid number. If one wasn't entered, ignore the input and 
    // then ask again, until a proper number is entered.
    while (1)
    {
      fgets(in_number_str, sizeof(in_number_str), stdin);
      if (sscanf( in_number_str, "%d", &in_number) == 1) break;
      printf("I cannot read that as a number: %s, try again.\n", in_number_str);
    }
    printf("-->%s, the number you entered is %d.\n", in_name, in_number);
    printf("---------------------------\n");
    // third time. Let's see whatwe get this time
    printf("Third time: \nWhat is your name? : ");    // Ask for user name
    fgets( in_chars_str, sizeof(in_chars_str), stdin);    // Read in enough to fill the input buffer
    sscanf( in_chars_str, "%s", in_name);  // extract the text minus the line feed
    // printf("More letters read: %d, string read in: %s\n", letters_read, in_chars_str);  // Debug: display input
    // printf("((--in_chars_str: %s, in_name_str: %s, Letters_read: %d))\n", in_chars_str, in_name, letters_read); // Debug: show what was entered
    printf("And what is the number this time? :");                  // Ask user for a number

    // Same read type as above but look for a valid number. If one wasn't entered, ignore the input and 
    // then ask again, until a proper number is entered.
    while (1)
    {
      fgets(in_number_str, sizeof(in_number_str), stdin);
      if (sscanf( in_number_str, "%d", &in_number) == 1) break;
      printf("I cannot read that as a number: %s, try again.\n", in_number_str);
    }
   
    printf("-->%s, the number you entered is %d.\n", in_name, in_number);

    return 0;
}