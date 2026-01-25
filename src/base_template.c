/*  base_template.c
    This program is a base template for future programs written my me.
    It is a basic template that does some initial startup stuff that I 
    like to have in programs, such as:
    - Program information at run time
    - input and output files, if any
    - startup parameters from the input line of configuration file, if any
    - any other basic setup and output that I might want up front.
    - This is merely an outline program and not complete. Only a skeleton for layout and method.
*/

// Turn off the MS-oriented secure function call warnings. They are considered near-useless anyway
// #define  _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>                         // C Standard library
#include <stdio.h>                          // Standard I/O functions
#include <string.h>                         // String manipulation
#include <time.h>                           // Times and time strings
#include <ctype.h>                          // For detecting specific character types

// include "timeDate.h"

/* Control and processing values (Shamelessly copied from some programs
   I had produced while still gainfully employed so many years ago.
   Some basic input and control buffers, vars, etc. for this program
   that may be referenced throughout this listing                
*/

/* Debugging flags with starting default values */
int  debug_buf      = 0;                      // Flag: Create a debug output file
int  debug_verbose  = 0;                      // Flag: Produce verbose debug information
int  debug_all      = 0;                      // Flag: Produce all available debug information
int  debug_disp_rec = 0;                      // Flag: Display input record

/* ----------------------------------------------------------------------------------------- 
   Default values if this program runs without input parms or a config file.
   All of these default values are used in case no applicable run-time values are 
   present as run-time parameters or values specified in the runtime configuration
   file (if there is one present).

   Note: These values are used or changed according to what the program is intended to do 
  ----------------------------------------------------------------------------------------- */

char def_cfg_filename    [] = "cfg.txt\0";            // Default run-time configuration file name
char def_log_filename    [] = "log.txt\0";            // Default output log file name - 'just the essentials'
char def_detlog_filename [] = "verbose_log.txt\0";    // Default log file name for verbose log data
char def_output_filename [] = "output.txt\0";         // Default output data

// Default paths:
char def_path            [] = "./\0";                 // Default generic path for files
char def_data_path       [] = "../data/\0";           // Default path to the input data file location
char def_output_path     [] = "../output/\0";         // Default path to the pgm output file(s)
char def_log_path        [] = "../log/\0";            // Default path to the pgm output log(s)
char def_debug_path      [] = "../log/\0";            // Default path to the pgm's debug listing files

/* Derived file names at run-time. Derived or set at run-time 
   Default values will be inserted, using the program run-time name and the applicable default values (above)
   These are set when the applicable files are opened.
*/
char pgm_name[40];                                    // Program name minus leading non-alpha chars
char cfg_input          [100];                        // Runtime configuration file name
char log_file           [100];                        // Run-time processing log output file name
char log_verbose        [100];                        // Run-time verbose debug log output file name
char output_file        [100];                        // Standard program processing output file name

/* Program name as determined at run-time. */
char input_file_basename[100];                // 1st part of input file name
char input_file_name[100];                    // Input file to this program
char full_input_file_name[100];               // Fully qualified input file name


/* Time/date values in a couple of formats.
   Hint: Get some alternative time/date ideas from the personal program timeDate.c  

   Note: These fields/values are defined globally in case they are used beyond the main() function.
*/
  time_t   system_Time;                       // Local copy of the system raw time at program start
  struct  tm loc_start_Time;                  // Local formatted copy of the derived local time
  struct  tm UTC_start_Time;                  // Local formatted copy of the derived GMT/UTC time structure
  char    loc_Time_str[25];                   // char version of loc_Time (formatted as needed)
  char    UTC_Time_str[25];                   // char version of UTC_Time (formatted as needed)
  char    gen_datetimestr[50];                // Char version - working date/time string, formatted and used as needed

  char    loc_start_Time_str[25];             // local date & time string (used later)
  char    UTC_start_Time_str[25];             // GMT/UTC date & time string (used later)
  char    gen_datetimestr[50];                // generic date/time string
 



/* Function profiles */
int set_runtime_parms(int,   char *[]);       // Read the runtime parms
int extract_parm(int, char * []);             // Extract and edit an individual parm
int parse_parm  (int, char * []);             // Parse a command-line parameter

/*------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  // int i   = 0;                                             // Generic increment counter
  int ret = 0;                                             // Generic return code reader
  char *cptr = NULL;
  // --------------------------------------------------------------------------------------------------------------------------------
  /* Initialize time/date fields from system calls */

  // --------------------------------------------------------------------------------------------------------------------------------
  /* The following is kept as a nice idea if you want to get the time value(s) strictly within the calling function */
  // time_t   system_Time = time(NULL);                         // define and init a local copy of the system raw time 
  // struct  tm loc_Time  = *localtime(&system_Time);           // define and init a local copy of the derived local time
  // struct  tm UTC_Time  = *gmtime(&system_Time);              // define and init a local copy of the derived GMT/UTC time structure
  // --------------------------------------------------------------------------------------------------------------------------------

  system_Time = time(NULL);                                     // define and get a copy of the system raw time at run-time
  loc_start_Time  = *localtime(&system_Time);                   // type tm: Get the local time from 'system_Time'
  UTC_start_Time  = *gmtime(&system_Time);                      // type tm: Get the GMT time from 'system_Time'
  
  memset(loc_start_Time_str, '\0', sizeof(loc_start_Time_str)); // initialize first 
  memset(UTC_start_Time_str, '\0', sizeof(UTC_start_Time_str)); // initialize first
  memset(gen_datetimestr,    '\0', sizeof(gen_datetimestr));    // initialize first

  strncpy(loc_start_Time_str, asctime(&loc_start_Time), 24);    // create basic local time date/time string, leave out the trailing '\n'
  strncpy(UTC_start_Time_str, asctime(&UTC_start_Time), 24);    // create basic GMT date/time string, leave out the trailing '\n'

  /* First, some basic program info, because it's available and I can get it */
  memset(pgm_name, '\0', sizeof(pgm_name));                     // Initialize, in case of initially dirty buffer
  
  /* Get the runtime program name from the argument list and strip off the leading navigation/directory chars
     We need to strip off the leading PATH chars which may contain BOTH non-char AND character directory names.. 
     Strip off everything up to and including the last '/' character
  */
  // i = 0;
  /*
  while ( isalnum(argv[0][i] ) == 0)                            // Detect non-alphanumeric char, skip it if found
    i++;
  */
  
  //Find the LAST occurance of the char '/'
  cptr = strrchr(argv[0], '/');
 
  // Debug: Display the resulting string, which should show the program name only, at run-time. 
  /*  
    if (cptr != NULL) 
    {
      printf("String after the last occurrence of '/': %s\n", cptr+1);
    } else
    {
      printf("Character not found.\n");
    }
  */
    
  strcpy(pgm_name, cptr+1);                                  // Keep the resulting program name
    
  printf("\n\nProgram: %s\n", pgm_name);                        // Display this program's name
  printf("Program Compile Date/Time: %s/%s\n", __DATE__, __TIME__);  // Display this program's compile date and time
  printf("Current time: (local): %s, ", loc_start_Time_str);    // Print out the 'local time' string
  printf("(UTC): %s", UTC_start_Time_str);                      // include UTC time to the string 
  printf("\n\n");                                               // finish line with line feed(s) 

  /* Set defaults and get any further run-time parameters */
  ret = 0;
  ret = set_runtime_parms(argc, argv);
  if (ret == 1)                         // No runtime command line parameters included
  {
    printf("*** Running with only default parameters. \n");
  }
  else
    if (ret == 4)                       // Problem with runtime command line parm(s) - ending program now.
    {
      printf("**** Bad/Invalid runtime input - ending now. ****\n");

      return ret;
    }



  /* Now get the specific processing environment(s) vars that this program needs to work in,
     such as
     MariaDB, MYSql, etc... database vars, required GUI-based environmental settings, etc.
     But, don't forget the immediate stuff such as system time and date, file names, etc....
  */

  return ret;
} /* main */

int set_runtime_parms(int pgm_argc, char *pgm_argv[])
{
  /* ----------------------------------------------------------------------------------
  Get and parse the program's runtime parameters.
  Get the configuration file's parameters
  Priority for settings are:
   3) Runtime command line parameter (highest)
   2) Configuration file setting(s)
   1) Default (lowest)
  Resolve them with the default values.
  Open the program's log file using the resolved file name.
  Open the program's detailed log file, if wanted, also using the resolved file name.
  Set any other runtime input parameters.
    ---------------------------------------------------------------------------------- */
  /* Set default runtime values. These *may* be overridden with runtime program parms or input config file info */
  // Note: pgm_name is already set (above)...
  
  /* First, initialize everything to NULL */
  memset(cfg_input,   '\0', sizeof(cfg_input));    
  memset(log_file,    '\0', sizeof(log_file));    
  memset(log_verbose, '\0', sizeof(log_verbose));    
  memset(output_file, '\0', sizeof(output_file));    

  /* Set the default values */ 
  /* By default, look for the default configuration in the same directory as the program is run from */
  //sprintf(cfg_input,   "%s%s_%s", def_path, pgm_name, def_cfg_filename);
  sprintf(cfg_input,   "%s_%s", pgm_argv[0], def_cfg_filename);              // Default config file location

  // Remaining default output files - Format: [default path][program name]_[applicable file name]
  // Note: By default the location will be the present working directory (ie. './')
  sprintf(log_file,    "%s%s_%s", def_path, pgm_name, def_log_filename);     // Default pgm log file location
  sprintf(log_verbose, "%s%s_%s", def_path, pgm_name, def_detlog_filename);  // Default pgm's detailed log location
  sprintf(output_file, "%s%s_%s", def_path, pgm_name, def_output_filename);  // Default pgm's provessing log location

  // First read in any run-time parameters and parse them.

  int param = 1;            // .start with the 2nd parm - after the program name - '0'

  // printf("Program parameters (strings) at run time:\n");

  printf("Parameter count after program name: %d\n", pgm_argc-1);
  //  If too many parameters, end with an error message and return immediately 
  if (pgm_argc > 10)
  {
    printf("*** Too many run-time parameters (%d). Not even trying... ***\n", pgm_argc);

    return 4;
  }




  param = 1;                      // start with the second parm (after the program name)
  while (param < pgm_argc)
  {
    printf("Parameter %d is %s\n", param, pgm_argv[param]);
    /* Parse the input parm */




    param++;
  }



  if (param > 1)
    return 0;             // normal return
  else
    return 1;             // return, code indicates no parameters

  printf("Searching for configuration file: %s", cfg_input);




  printf("\n\n");                                               // finish line with line feed(s) 
} /* set_runtime_parms */
 
/* ----------------------------------------------------------------------------- */
/* parse_parm: Extract and parse the input parm                                  */
/*                                                                               */
/* ----------------------------------------------------------------------------- */
int parse_parm(int p_argc, char * p_argv[])
{
  char *ch;    /* char string pointer for loop */
  int i;       /* loop increment */

  /* Look for a hyphenated parameters and parse it */
  if (strncmp( p_argv[1], "-", 1) == 0 )
  {
    printf(" *** Runtime parm: <%s>\n", p_argv[1]);

    // Set debug flags







    if ( (strncmp( p_argv[1], "-c=", 3) == 0 ) || (strncmp( p_argv[1], "-C=", 3) == 0 )  )
    {
      // Activate/de-activate frequent file buffer flushes
      if      ( (strncmp( (p_argv[1] + 3), "B-", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "b-", 2) == 0 )  )
      {
        // Switch OFF frequent buffer flushes
        debug_buf = 0;       /* Produce program debug trace information */
        printf(" *** Debug: Switch OFF frequent buffer flushes ****\n");
      }
      /*
      else if ( (strncmp( (p_argv[1] + 3), "B+", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "b+", 2) == 0 )  )
      {
        // Switch ON frequent buffer flushes
        debug_buf = 1;       // Produce program debug trace information
        printf(" *** Debug: Flush buffers frequently ****\n");
      }

      // Activate/de-activate displaying inout row contents before before processing
      else if ( (strncmp( (p_argv[1] + 3), "D-", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "d-", 2) == 0 )  )
      {
        // Switch OFF displaying input record
        debug_disp_row = 0;       // Produce program debug trace information
        printf(" *** Debug: Switch OFF displaying rows before INSERT/UPDATE ****\n");
      }

      */

      else
      {
        printf(" ===== Invalid debug switch type: %s =====\n",p_argv[1]);
        return(1);
      }
    }

    /* Parse input file name */
    else if ( (strncmp( p_argv[1], "-i=", 3) == 0 ) || (strncmp( p_argv[1], "-I=", 3) == 0 )  )
    {
      memset(input_file_name,'\0',sizeof(input_file_name));
      memset(input_file_basename,'\0',sizeof(input_file_basename));
      memset(full_input_file_name,'\0',sizeof(full_input_file_name));

      // Get the base name of the input file name -  everything except the last part
      ch = p_argv[1]+3;
      for (i=0; i < strlen(ch); i++)
      {
        if (ch[i] != '.')
          input_file_basename[i] = ch[i];
        else
          if ( (strcmp(&ch[i],".txt") == 0) || (strcmp(&ch[i],".dat") == 0) )
          {
            break;
          }
          else
          {
            input_file_basename[i] = ch[i];
          }
      }
      // printf(" base name: <%s>\n",input_file_basename);

      // Construct the fully-qualified input file name
      strcpy(full_input_file_name,def_data_path);
      strcat(input_file_name,p_argv[1]+3);
      strcat(full_input_file_name,p_argv[1]+3);
      /*
      printf(" *** Input file: %s\n",input_file_name);
      printf(" *** Full input file: %s\n",full_input_file_name);
      */
    }
    else
    {
      printf(" ===== Invalid switch type: %s =====\n",p_argv[1]);
      return(10);
    }
    /*
    else
     // Code any extra flags here.
     ** By default, skip any unknown flags
    */

    return(0);
  }
  else
  {
    printf(" ===== Invalid switch type: %s =====\n",p_argv[1]);
    return(10);
  }

  return(0);
} /*-- parse_parm */