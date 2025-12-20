/* ===================================================================
**
** Program: msglog.c
** Written by: Edward Morawski
** Date Written: Novenber 2025
**
** This program will append to the specified file a free-form text message with the current system time.
**
** Usage: To log freeform messages with a timestamp to a file.
**
** Required parameters:
** -----------
**    parm1 = fully qualified file name (output)
**    parm2 = 'free-form comment' (Enclosed within single quotes)
**
** Execution of the program
** ------------------------
** msglog <file name> '<text message>'
** 
**
** Note: The message will be copied to the output log file without the quotes.
** ====================================================================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *u_time_date (void);      /* Procedure prototype */

/*============================================================ */
/* main                                                        */
/*============================================================ */
 int main(int argc, char *argv[])
{
  char file_name[100];
  char str_txt[151];
  char nowdate[30];
  FILE *file_ptr = NULL;

  if (argc != 3 )
  {
    printf("\n Proper program usage:\n\n");
    printf("    msglog <file> <msg>\n\n");
    printf("         where: <file>  - log file to append the message to\n");
    printf("                '<msg>' - text message to append (within quotes)\n\n\n");
    return(-1);
  }
  memset(file_name, '\0', sizeof(file_name));       /* initialize */
  memset(str_txt, '\0', sizeof(str_txt));           /* initialize */
  strcpy(file_name, argv[1]);                       /* get the file name */
  if( (file_ptr=fopen(file_name,"a")) == NULL )
  {
    printf("\nCan not open input file -> %s \n", file_name);
    exit(-1);
  }
  strcpy(nowdate, u_time_date());                   /* get the system time       */
  strcpy(str_txt, argv[2]);                         /* get the input text string */
  fprintf(file_ptr,"%s: %s\n", nowdate, str_txt);   /* append to file            */
  printf(" Logged to file '%s': %s: %s\n", file_name, nowdate, str_txt);    /* Display to screen         */

  fclose(file_ptr);                                /* finish up                  */

  return 0;
} /*-- main */

/*****
 * Return the system date & time in timestamp format AAAA-MM-JJ HH:MM:SS
 *****/
char *u_time_date (void)
{
  static char str_time[30];

  struct tm *ptr_time;
  time_t l_time;

  l_time = time(&l_time);
  ptr_time = localtime(&l_time);

  sprintf(str_time,"%04d-%02d-%02d %02d:%02d:%02d",
	  ptr_time->tm_year + ((ptr_time->tm_year < 50) ? 2000 : 1900),
          ptr_time->tm_mon + 1,
          ptr_time->tm_mday,
	  ptr_time->tm_hour,
	  ptr_time->tm_min,
	  ptr_time->tm_sec);

  return (str_time);
} /*-- u_time_date */
