/* ----------------------------------------------------------------------------------------------------------------------
  base_template.c
    Author: Edward Morawski

    This program is a C Language base template for future programs written my me.
    It does some initial startup stuff that I like to have in my programs,
    such as:
    - Display some program startup information at run time
    - Open any input and output files, if any
    - Set basic values and text strings from any runtime parameters and from the program's configuration file, if any
    - Any other basic setup and output that may be a good idea to have
    
    As such, this is a runnable program - a skeleton for future program coding.

    Note: Before you say anything about the code style/practices: Yes, some of the following is 'old school' practice
          and meant for clarity of code and purpose. If you want to be 'fancy' in your code, you risk clarity in process
          flow and you don't want to be guessing about what's happening 6+ months after you thought you were done and must 
          re-visit this module. Gives whoever follows a break.

   ----------------------------------------------------------------------------------------------------------------------*/

// Turn off the MS-oriented secure function call warnings. They are considered near-useless anyway
// #define  _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>                         // C Standard library
#include <stdio.h>                          // Standard I/O functions
#include <string.h>                         // String manipulation
#include <time.h>                           // Times and time strings
#include <ctype.h>                          // For detecting specific character types

/* Debugging flags with starting default values */
int  debug_buf      = 0;                    // Flag: Do frequent debug/log file flushes
int  debug_log      = 0;                    // Flag: Create a debug output file
int  debug_verbose  = 0;                    // Flag: Produce verbose debug information
int  debug_all      = 0;                    // Flag: Produce all available debug information
int  debug_disp_rec = 0;                    // Flag: Display input record

/* ----------------------------------------------------------------------------------------- 
   Default file names. May be updated via runtime parameters of values in a config file.
  ----------------------------------------------------------------------------------------- */
char def_cfg_filename       [] = "cfg.txt\0";         // Default run-time configuration file name
char def_log_filename       [] = "log.txt\0";         // Default output log file name - 'just the essentials'
char def_verboselog_filename[] = "verbose_log.txt\0"; // Default log file name for verbose log data
char def_output_filename    [] = "output.txt\0";      // Default output data

// Default paths - the expected relative locations of the input files:
char def_path            [] = "./\0";                 // Default generic path for files
char def_log_path        [] = "../log/\0";            // Default path to the pgm output log(s)
char def_verboselog_path [] = "../log/\0";            // Default path to the pgm's verbose processing log file
char def_data_path       [] = "../data/\0";           // Default path to the input data file location
char def_output_path     [] = "../output/\0";         // Default path to the pgm output file(s)

// Flags to indicate if the above files are actually opened. Default value is 'no'
int  cfg_open        = 0;
int  log_open        = 0;
int  verboselog_open = 0;
int  output_open     = 0;

/* Derived file names at run-time. Set at run-time
   Default values will be inserted, using the program run-time name and the applicable default values (above)
   These are set when the applicable files are opened.
*/
char pgm_name    [40];              // Program name minus leading non-alpha chars
char cfg_input   [100];             // Runtime configuration file name
char log_file    [100];             // Run-time processing log output file name
char log_verbose [100];             // Run-time verbose debug log output file name
char output_file [100];             // Standard program processing output file name

/* Program name as determined at run-time. */
char input_file_namebase [100];     // 1st part of input file name (path)
char input_file_name     [100];     // Input file to this program
char full_input_file_name[100];     // Fully qualified input file name

/* Time/date values in a couple of formats.
   Hint: Get some alternative time/date ideas from the personal program timeDate.c  
   Note: These fields/values are defined globally in case they are used beyond the main() function.
*/
time_t  system_Time;                // Local copy of the system raw time at program start
struct  tm loc_start_Time;          // Local formatted copy of the derived local time
struct  tm UTC_start_Time;          // Local formatted copy of the derived GMT/UTC time structure
char    loc_Time_str   [25];        // char version of loc_Time
char    UTC_Time_str   [25];        // char version of UTC_Time
char    gen_datetimestr[50];        // Char version - tmp date/time string, formatted and used as needed

char    loc_start_Time_str[25];     // local date & time string
char    UTC_start_Time_str[25];     // GMT/UTC date & time string
char    gen_datetimestr   [50];     // generic date/time string - tmp/working string

/* Function profiles */
int get_runtime_parms(int, char * []);    // Read the runtime parms
int extract_parm     (int, char * []);    // Extract and edit an individual parm
int get_parm         (int, char * []);    // Parse a command-line parameter

/*------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  int   ret  = 0;                         // Generic return code value
  char *cptr = NULL;                      // pointer to a char, actually a pointer to an embedded string

  // --------------------------------------------------------------------------------------------------------------------------------
  // Initialize time/date fields from system calls

  // --------------------------------------------------------------------------------------------------------------------------------
  /* The following is kept as a nice idea if you want to get the time value(s) strictly within the calling function */
  // time_t   system_Time = time(NULL);                         // define and init a local copy of the system raw time 
  // struct  tm loc_Time  = *localtime(&system_Time);           // define and init a local copy of the derived local time
  // struct  tm UTC_Time  = *gmtime(&system_Time);              // define and init a local copy of the derived GMT/UTC time structure
  // --------------------------------------------------------------------------------------------------------------------------------

  system_Time     = time(NULL);                                 // get a copy of the system raw time at run-time
  loc_start_Time  = *localtime(&system_Time);                   // type tm: Get the local time from 'system_Time'
  UTC_start_Time  = *gmtime(&system_Time);                      // type tm: Get the GMT time from 'system_Time'
  
  memset(loc_start_Time_str, '\0', sizeof(loc_start_Time_str)); // initialize/NULL first  
  memset(UTC_start_Time_str, '\0', sizeof(UTC_start_Time_str)); // initialize/NULL first
  memset(gen_datetimestr,    '\0', sizeof(gen_datetimestr));    // initialize/NULL first

  strncpy(loc_start_Time_str, asctime(&loc_start_Time), 24);    // create basic local date/time string, leave out the trailing '\n'
  strncpy(UTC_start_Time_str, asctime(&UTC_start_Time), 24);    // create basic GMT   date/time string, leave out the trailing '\n'

  /* First, some basic program info, because it's available and I can get it */
  memset(pgm_name, '\0', sizeof(pgm_name));                     // Initialize, in case of initially dirty buffer
  
  /* Get the basic runtime program name
     We need to strip off the leading PATH chars which may contain BOTH non-char AND character directory names.. 
     Strip off everything up to and including the last '/' character before the program runtime name
  */
    
  // Point to the LAST occurance of the char '/' before the program name
  cptr = strrchr(argv[0], '/');
 
  // Debug: Display the resulting string, which should show the program name only, at run-time. 
  /*
  if (cptr != NULL) 
        { printf("Basic program name: %s\n", cptr+1);   // String after the LAST occurance of '/' in the string } 
  else  { printf("Character not found.\n"); }
  */  
  strcpy(pgm_name, cptr+1);                                          // Save the resulting program name
  
  // Now display some startup information, to show that the program is actually running.
  printf("\n\nProgram: %s\n", pgm_name);                             // Display this program's name
  printf("Program Compile Date/Time: %s/%s\n", __DATE__, __TIME__);  // Display this program's compile date and time
  printf("Current time: (local): %s, ", loc_start_Time_str);         // Print out the 'local time' string
  printf("(UTC): %s", UTC_start_Time_str);                           // Display the UTC time to the string  
  printf("\n\n");                                                    // Finish line with line feed(s) 

  /* -------------------------------------------------------------------------------------------------
    Get all runtime parameters and then configuration file parameters to set the overall environment 

    Default settings are already hard-coded and may be overwritten this way.

     Process for setting defaults:
      1) Program already has hard-coded default values, to be used if no override is found
      2) Command line values override hard-coded values
      3) Configuration file settings override default values not already set by the command line parms 
         (ie. Command line parms always used)
     -------------------------------------------------------------------------------------------------*/
 
  ret = 0;                                          // Generic return code var
  ret = get_runtime_parms(argc, argv);              // Get whatever runtime/command line parameters are present
  if (ret == 1)                                     // If no runtime command line parameters included
  {
    ;
    // printf("*** No command line parameters. \n");   // Indicate that there were no runtime parms found.
  }
  else
    if (ret == 4)                                   // Problem with runtime command line parm(s) - ending program now.
    {
      printf("**** Bad/Invalid command line input - ending now. ****\n");

      return ret;
    }
    else
    {
      // Runtime parameters successfully read in
      //printf("++++ runtime parameters read in.++++\n");
      ;
    }

  // Runtime parms set. Now look for a configuration file.   
  printf("Searching for configuration file: %s\n\n", cfg_input);


  /* --------------   Do the config file thing... ----------------- */


/*
     Open the program's log file using the resolved file name.
     Open the program's detailed log file, if wanted, also using the resolved file name.
     Set any other runtime input parameters.
     Note: pgm_name is already set (above)...
*/


  /* Now get the specific processing environment(s) vars that this program needs to work in,
     such as
     MariaDB, MYSql, etc... database vars, required GUI-based environmental settings, etc.
     But, don't forget the immediate stuff such as system time and date, file names, etc....
  */









  return ret;
} /* main */

int get_runtime_parms(int pgm_argc, char *pgm_argv[])
{
  /* ----------------------------------------------------------------------------------
     Get any command line runtime parameter values. 
     These will set some run-time values and overright some default hard-coded values 
    
     Also note: All output here is to the command line - no files have yet been opened.
     ---------------------------------------------------------------------------------- */
  int rc     = 0;            // return code from call   
  int ret_rc = 0;            // Overall return code to main. If there is any issue, then this will be non-zero
  int param  = 1;            // parameter number - start with the 2nd parm - after the program name
  
  /* First, initialize default settings to NULL */
  memset(cfg_input,   '\0', sizeof(cfg_input));    
  memset(log_file,    '\0', sizeof(log_file));    
  memset(log_verbose, '\0', sizeof(log_verbose));    
  memset(output_file, '\0', sizeof(output_file));    

  /* -- First, set set the hard-coded default values */ 

  // By default, the program's configuration file is assumed to be in the same directory as the program is run from
  // Default file setup: The configuration is found in the present working directory (ie. './')
  //                     Log file output(s) will be put in the 'log' subdirectory of the present working directory
  //                     The input data file - if any - is found in the 'data ' subdirectory
  //                     The program output file(s) - if any - will be written out to the 'output' subdirectory

  sprintf(cfg_input,   "%s%s_%s", def_path,            pgm_name, def_cfg_filename);         // Set default config file name
  sprintf(log_file,    "%s%s_%s", def_log_path,        pgm_name, def_log_filename);         // Set default pgm log file location
  sprintf(log_verbose, "%s%s_%s", def_verboselog_path, pgm_name, def_verboselog_filename);  // Set default pgm's verbose log location
  
  // Commented out but not forgotten: No Default files - we don't know the names of the input or output files at this time.
  // sprintf(input_file,  "%s%s_%s", def_data_path,       pgm_name, def_input_filename);  // Set default pgm's regular log location
  // sprintf(output_file, "%s%s_%s", def_output_path,     pgm_name, def_output_filename);  // Set default pgm's regular log location

  // Now read in any run-time parameters and parse them.

  // printf("Program parameters (strings) at run time:\n");
  // printf("Parameter count after program name: %d\n", pgm_argc-1);

  ret_rc = 0;                             // will collect the highest return code found
  if (pgm_argc == 1)                      // will always have at least 1 parm: the program name
  {
    // printf("*** No command line (run time) parameters found ***\n");
    return 1;                            // That was easy: No further command line parms; return with 1
  }
  else
  {
    /* Parse out the runtime parameters */
    if (pgm_argc > 8)                     // Too many parameters - don't even try - return with error 16
    {
      // Too many parms - include this to flag a mistake such as an improperly set long string value
      printf("*** Too many (%d of them!) run-time parameters. Not even trying... ***\n", pgm_argc-1);

      return 16;
    }
    else
    {
      printf("Number of parameters: %d\n", pgm_argc -1);       // Show the number of parameters, excluding parm 0 (program name)
      /*  Parse each input parameter  */
      param = 1;                                // Intialize to read the first parameter after pgm name
      while ( param < pgm_argc )                // Go up the chain of parms and make sure you're not counting past the array
      {
        rc = get_parm(param,  pgm_argv);        // Get the next command line parm 
        if (rc != 0)
        {
          if (ret_rc < rc) ret_rc = rc;         // Collect the highest return code encountered in the loop
          if (rc > 4)                           // Very bad return code
          {
              // printf("Parameter %d, Unusual: Return code: %d, Parameter: <%s>\n", param, rc, pgm_argv[param]);
              ret_rc = rc;
            
              return ret_rc;
          }
          else
          {
              // bad return code 

              printf("Parameter %d is invalid (ignored): <%s>, Return code: %d.\n", param, pgm_argv[param], rc);
          }
        }
        else
        {
            ;         // Good parameter nothing more to do
        }

        ++param;                                // Point to the next parameter
      } /* while ( param < pgm_argc ) */
    }  // else
  } // else
 
  printf("+++ Done reading parameters ++++\n"); 

  return 0;
} /* get_runtime_parms */
 
/* ----------------------------------------------------------------------------- */
/* get_parm: Extract and parse the command line input parameters                 */
/*                                                                               */
/* ----------------------------------------------------------------------------- */
int get_parm(int parm_num, char * p_argv[])
{
  char *ch;                      /* char string pointer for loop */
  int i;                         /* loop increment               */

  // printf(" +++ Parameter %d: <%s>\n", parm_num, p_argv[parm_num]);

  /* Look for a hyphenated parameter and parse it */
  if  ( (strncmp( p_argv[parm_num], "-", 1) == 0 )  ||
        (strncmp( p_argv[parm_num], "/", 1) == 0 )   )
  {
    printf(" Parameter %d: <%s>\n", parm_num, p_argv[parm_num]);



    // Set debug flags



    return 0;





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
      memset(input_file_namebase,'\0',sizeof(input_file_namebase));
      memset(full_input_file_name,'\0',sizeof(full_input_file_name));

      // Get the base name of the input file name -  everything except the last part
      ch = p_argv[1]+3;
      for (i=0; i < strlen(ch); i++)
      {
        if (ch[i] != '.')
          input_file_namebase[i] = ch[i];
        else
          if ( (strcmp(&ch[i],".txt") == 0) || (strcmp(&ch[i],".dat") == 0) )
          {
            break;
          }
          else
          {
            input_file_namebase[i] = ch[i];
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
      // Invalid parameter. The parm value is not a is not a defined type 
      printf(" =+=+= Invalid switch type: %s =+=+=\n",p_argv[1]);
      return(4);
    }
    /*
    else
     // Code any extra flags here.
     ** By default, skip any unknown flags
    */

  }
  else
  {
    // Invalid parameter type. It was not prefixed with '-' or '/' 
    // Return with a return code 4
    // This will allow for extracting & validating further parameters

    // printf("What is this?? ");
    // printf(" ===== Invalid/unknown switch type: %s =====\n",p_argv[parm_num]);
    return(4);
  }


  return(0);
} /*-- get_parm */