/* ----------------------------------------------------------------------------------------------------------------------
  base_template.c
    Version: 1.0  (June 2026)
    Author: Edward Morawski

    This code is a generic, runnable C-language program to exercise run-time configuration through command line 
    parameters together with configuration file parameter input.
    
    It also serves as a personal template for future programs regarding parameter processing.

    This program will: 
    - Start with a displayu of some startup information.
    - Collect runtime command line parameters, followed by read a configuration file, if there is one.
      This will collect any run-time settings for the program run - (ie. working directories, files to open, debug output level, etc)
    - Any other generic basic setup work that might be a good idea to have.
    There is also some rudimentary code to accomodate basic data input and program output files. There are NO defaults
      for these files. If you need to use input and/or output file(s), names absolutely need to be added in.
    
    This is a 'live' file and will be modified as needed.

    Note: Before you judge coding style:
        1) Yes, it is over-engineered but meant to cover a wide range of potential coding needs
        2) Yes, the code style is definitely 'old school'. It's my style. 
        3) There are a LOT of comments. It's for helping with code clarity long after I am done with this code.
   
   Updates:
    June 2026: Version 1.0  - initial version
   ----------------------------------------------------------------------------------------------------------------------*/

// Turn off the MS-oriented secure function call warnings. They are considered near-useless anyway
// #define  _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>                         // C Standard library
#include <stdio.h>                          // Standard I/O functions
#include <string.h>                         // String manipulation
#include <time.h>                           // Times and time strings
#include <ctype.h>                          // Detect specific character types

// Default paths - the expected *relative* locations of pre-coded files:
// By default it's assumed that the program is being run from its 'bin' directory
char def_path             [] = "./\0";              // Default generic path for files
char def_log_path         [] = "../log/\0";         // Default log file path - output log(s)
char def_in_path          [] = "../data/\0";        // Default input path - input data file(s)
char def_out_path         [] = "../output/\0";      // Default output path - output file(s)

/* For future consideration: The file names below are left 'bare' in case a name 
   qualifier is needed for uniqueness among multiple runs
*/
char def_name_trailer_txt [] = ".txt\0";        // default text file name trailer

/* ----------------------------------------------------------------------------------------- 
   Default file names. May be updated via runtime parameter(s) or a config file.
  ----------------------------------------------------------------------------------------- */
char def_cfg_filename     [] = "cfg\0";         // Default run-time configuration file name
char def_log_filename     [] = "log\0";         // Default output log  name - 'just the essentials'
char def_debuglog_filename[] = "verbose_log\0"; // Default output file name for verbose log and debug output
char def_in_filename      [] = "input\0";       // Default input data file
char def_out_filename     [] = "output\0";      // Default output data file

// Files (in their final form) to work with 
char cfg_file      [100];         // Configuration file name
char log_file      [100];         // Log output file name
char debuglog_file [100];         // Verbose/Debug log output file name
char in_file       [100];         // Program input file name
char out_file      [100];         // Program output file name

/*  File handles */
FILE *cfg_file_ptr      = NULL;   // Input - configuration file
FILE *log_file_ptr      = NULL;   // Log output file name
FILE *debuglog_file_ptr = NULL;   // Verbose/Debug log output file name
FILE *in_file_ptr       = NULL;   // Input data file name
FILE *out_file_ptr      = NULL;   // Program output file name

// Flags to indicate if the above files are actually opened. Default value is 0 (ie. 'no')
int  cfg_open      = 0;
int  log_open      = 0;
int  debuglog_open = 0;
int  in_open       = 0;
int  out_open      = 0;

// Flags to indicate where file names were set.
// Show that it was set and at what stage of startup. This will help enforce the practice that it is set only once.
// Valid values: ' ' - Not set at all or default (no value)
//               'd' - default - use the program's hard-coded values
//               'r' - set through a run-time parameter
//               'c' - set through a configuration file setting
char cfg_open_set       = 'd';    // Flag: configuration file name
char log_open_set       = 'd';    // Flag: standard program output log name
char debuglog_open_set  = 'd';    // Flag: debug file name
char in_open_set        = ' ';    // Flag: data input file name - not pre-defined by default
char out_open_set       = ' ';    // Flag: data output file name - not pre-defined by default

/* Debugging flags with starting default values ( Default - 0 (ie. 'no'/'none' )  */
int  debug_flush        = 0;      // Perform frequent debug/log file buffer flushes (0 - no-rapid flushes, 1 - frequent buffer flushes)
int  debug_log          = 0;      // Create a debug file and set its verbosity (0 - no debug file; 1- basic; 2- detailed; 3- All details available)
int  pgm_log            = 0;      // Create a standard program log file (0 -no log file, 1 - Write a log file (default) )

char pgm_name       [40];         // Unqualified program name at run-time - minus leading non-alpha chars

/* Time/date values in a couple of formats.
   Note these fields/values are defined globally in case they are used beyond the main() function.
   Hint: You'll find some alternative time/date ideas from the personal program timeDate.c  
*/
time_t  system_Time;              // Local copy of the raw system time
struct  tm loc_start_Time;        // Local time - time structure to hold local time derived from system_Time
struct  tm UTC_start_Time;        // GMT/UTC time - time structure to hold UTC time derived from system_Time
char    *loc_Time_str    ;        // Pointer to derived local time string - derived from loc_start_Time
char    *UTC_Time_str    ;        // Pointer to derived UTC_Time string - derived from UTC_start_time

char    loc_start_Time_str[25];   // Program start - local time & date string -ess trailing '\n' character
char    UTC_start_Time_str[25];   // Program start - GMT/UTC date & time string - less trailing '\n' character
char    gen_datetimestr   [50];   // generic date/time string - tmp/working string - less trailing '\n' character

/* Function profiles */
/*------------------------------------------------------------------------------------------------------------------------------------*/
int get_runtime_parms(int, char *[] );    // Read the runtime parms
int get_config_parms ()              ;    // Get whatever configuration file parameters are present
int extract_param    (char [] )      ;    // Extract and edit an individual parameter from a string
int trimspaces       (char *)        ;    // Trim all leading and trailing spaces and non-display chars from a string

/*------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  int   ret  = 0;                         // Generic return code value from called functions
  char *cptr = NULL;                      // Pointer to a char, actually a pointer to an string

  // --------------------------------------------------------------------------------------------------------------------------------
  // Initialize time/date fields from system calls
  // --------------------------------------------------------------------------------------------------------------------------------

  // --------------------------------------------------------------------------------------------------------------------------------
  /* The following is kept aside as a nice idea if you want to get the time value(s) strictly within the function */
  // time_t   system_Time = time(NULL);                         // define and init a local copy of the system raw time 
  // struct  tm loc_Time  = *localtime(&system_Time);           // define and init a local copy of the derived local time
  // struct  tm UTC_Time  = *gmtime(&system_Time);              // define and init a local copy of the derived GMT/UTC time structure
  // --------------------------------------------------------------------------------------------------------------------------------
  
  /* First, some basic program info, because it's available and I can get it */
  memset(loc_start_Time_str, '\0', sizeof(loc_start_Time_str)); // initialize/NULL first  
  memset(UTC_start_Time_str, '\0', sizeof(UTC_start_Time_str)); // initialize/NULL first
  memset(gen_datetimestr,    '\0', sizeof(gen_datetimestr));    // initialize/NULL first

  system_Time     = time(NULL);                                 // get a copy of the system raw time at run-time
  loc_start_Time  = *localtime(&system_Time);                   // type tm: Get the local time from 'system_Time'
  UTC_start_Time  = *gmtime(&system_Time);                      // type tm: Get the GMT time from 'system_Time'

  strncpy(loc_start_Time_str, asctime(&loc_start_Time), 24);    // copy local date/time string, leave out the trailing '\n'
  strncpy(UTC_start_Time_str, asctime(&UTC_start_Time), 24);    // copy GMT   date/time string, leave out the trailing '\n'

  /* Get the basic runtime program name, leaving out all leading PATH chars which may contain BOTH non-char AND character directory names.. 
  */
  memset(pgm_name, '\0', sizeof(pgm_name));    
  cptr = strrchr(argv[0], '/');                         // Point to the LAST occurance of the char '/' before the program name
  strcpy(pgm_name, cptr+1);                             // Save the remaining string chars - program name 
  
  // Debug: Display the resulting string, which should show the program name only, at run-time. 
  /*
  if (cptr != NULL) 
        { printf("Runtime program name: %s\n", cptr+1);   // String after the LAST occurance of '/' in the string } 
  else  { printf("Character not found.\n"); }
  */  
  
  /* -------------------------------------------------------------------------------------------------
    Get all command line and configuration file parameters to set specific overrides.
    This is the override precidence:
      1) Program already has hard-coded default values. Those will be used if no corresponding override is found.
      2) Command line parms will override default values.
      3) Configuration file parameters will override default values not already overridden by the command line parms.
     -------------------------------------------------------------------------------------------------
  */
   // Get command line parameters
  ret = 0;
  ret = get_runtime_parms(argc, argv);              // Get whatever runtime/command line parameters are present
  switch (ret)                                      // look at the return code
  {
    case 0 :  break;                                // do nothing - it's all good
    case 1 :  printf("\n * No command line parameters. *\n");   // Indicate that there were no runtime parms found.
              break;
    
    case 2 :  break;
    
    case 3 :  break;
    
    default:  printf("\n **** Bad/Invalid command line input - ending now. rc=%d****\n", ret);
              return ret;                                   // Yes, quit the program

              break;
  } // switch (ret)

  /* --------------   Read and process the config file (if present)   ----------------- */
  /* Look for the configuration file for this program run.
  * Use the run-time command line parameter value for this, if it is present. Otherwise, use the default name.
  * If a configuration file is found, open it and process its contents for final configuration 
  * If there is no configuration file to be found,
  *        a) If a file was specified and not found, end the program with an error message
  *        b) If no parameter file was specified, use the hard-coded default parameter file name. 
  *           If the default file was not found, then carry on with internal default values.
  */

  ret = 0;                                          // Generic return code var
  ret = get_config_parms();                         // Get whatever configuration file parameters are present
  switch (ret)                                      // look at the return code
  {
    case 0 :  break;                                // Do nothing - it's all good
    
    case 1 : 
              // No default file was found and no alternate specified - go with pgm defaults
              printf(" Default configuration file <%s> not found. Going with coded defaults.\n", cfg_file);
              
              break;                                

    case 2 : 
    
              break;

    case 3 : 
              break;
  
    case 4 :  printf(" Configuration file error - ending now. RC: %d\n", ret);

              return ret;
              break;
  
    default:  printf(" Configuration error - ending now. RC: %d\n", ret);   // Program panic
              
              return ret;
              break;
  } // switch
  
  /* --------------   Do the log file(s) thing... ----------------- */
  /*
     Open the program's log file
     Open the program's detailed (ie. debug) log file, if wanted,
     Set any other runtime input parameters.
     Note: pgm_name is already set (above)...
     Once the log file and the debug file (if any) are opened, initialize them with
     some opening output, including run-time config information
  */




  // Screen display some startup information to show that the program is actually running.
  printf("\n\n");                                                    // Separate output from any preceeding terminal text 
  printf("Program: %s\n", pgm_name);                                 // Display this program's name
  printf("Program Compile Date/Time: %s/%s\n", __DATE__, __TIME__);  // Program's compile date and time
  printf("Current time: (local): %s, ", loc_start_Time_str);         // Run start time - 'local time' string
  printf("(UTC): %s", UTC_start_Time_str);                           // Run start time - UTC time string  
  printf("\n\n");                                                    // Finish initial output with line feed(s) 




  if (pgm_log)                                              // If you want an ouput log file
  {
    if( (log_file_ptr = fopen(log_file,"w")) == NULL )      // Try to open the output log file 
    {
      // If we cannot open an output log file the program should halt running in this case.
      // We want to see this message.
      printf(" Program log file cannot be opened: %s Cannot continue.\n", log_file);

      return(4);
    }
    else
    { 
      // Display some startup information.
      fprintf(log_file_ptr, "=================================================================================\n");
      fprintf(log_file_ptr, "Program: %s\n", pgm_name);                                 // Display this program's name
      fprintf(log_file_ptr, "Program Compile Date/Time: %s/%s\n", __DATE__, __TIME__);  // Program's compile date and time
      fprintf(log_file_ptr, "Current time: (local): %s, ", loc_start_Time_str);         // Run time - 'local time' string
      fprintf(log_file_ptr, "(UTC): %s\n", UTC_start_Time_str);                         // Run time - UTC time string  
      fprintf(log_file_ptr, "=================================================================================\n");
      fprintf(log_file_ptr, "\n\n");                                                    // Finish initial output with line feed(s) 


      // Add the config & debug values here


    }
  }

  if (debug_log)                                              // If you want an debug log file
  {
    if( (debuglog_file_ptr = fopen(debuglog_file,"w")) == NULL )      // Try to open the output log file 
    {
      // If we cannot open an output log file the program should halt running in this case.
      // We want to see this message.
      printf(" Program log file cannot be opened: %s Cannot continue.\n", log_file);

      return(4);
    }
    else
    { 
      // Display some startup information.
      fprintf(log_file_ptr, "============================= Debug Output ======================================\n");
      fprintf(log_file_ptr, "Program: %s\n", pgm_name);                                 // Display this program's name
      fprintf(log_file_ptr, "Program Compile Date/Time: %s/%s\n", __DATE__, __TIME__);  // Program's compile date and time
      fprintf(log_file_ptr, "Current time: (local): %s, ", loc_start_Time_str);         // Run time - 'local time' string
      fprintf(log_file_ptr, "(UTC): %s\n", UTC_start_Time_str);                         // Run time - UTC time string  
      fprintf(log_file_ptr, "============================= Debug Output ======================================\n");
      fprintf(log_file_ptr, "\n\n");                                                    // Finish initial output with line feed(s) 

      // Add the config & debug values here
    }
  }



  /* --------------   Do more config stuff... ----------------- */
  /* Now set up any specific processing configuration and setup, such as MariaDB/MYSql, database connection, 
     GUI-based environmental settings, etc.
  */



  /* --------------- Now get to work, starting here ----------------- */




  if (pgm_log)
    fclose(log_file_ptr);           // Program end - if the log file was opened, then close it
  if (debug_log)
    fclose(debuglog_file_ptr);      // Program end - if the debug log file was opened, then close it

  return ret;           // End the program with whatever return code is intended 
} /* main */

int get_runtime_parms(int pgm_argc, char *pgm_argv[])
{
  /* ----------------------------------------------------------------------------------
     Firstly, set the default switch values, then look for command line parameters.
     The run-time parameters will overwrite corresponding default values. 
    
     Also note: All output here is to the command line - no files have yet been opened.
     ---------------------------------------------------------------------------------- */
  int rc         = 0;            // generic return code from function call   
  int highest_rc = 0;            // Overall return code to main. If there is any issue, then this will be non-zero
  int param      = 1;            // parameter number - start with the 2nd parm - after the program name
  
  /* First, initialize default file name strings to NULL */
  memset(cfg_file,     '\0', sizeof(cfg_file));    
  memset(log_file,     '\0', sizeof(log_file));    
  memset(debuglog_file,'\0', sizeof(debuglog_file));    
  memset(in_file,      '\0', sizeof(in_file));    
  memset(out_file,     '\0', sizeof(out_file));    

  /* -- First, set set the hard-coded default values */ 

  // By default, the program's configuration file is assumed to be in the same directory as the program is run from
  // Default file setup: The configuration is found in the present working directory (ie. './')
  //                     Log file output(s) will be put in the 'log' subdirectory of the present working directory
  //                     The input data file - if any - is found in the 'data ' subdirectory
  //                     The program output file(s) - if any - will be written out to the 'output' subdirectory

  sprintf(cfg_file,      "%s%s_%s%s", def_path,     pgm_name, def_cfg_filename, def_name_trailer_txt);  // Default config file name
  sprintf(log_file,      "%s%s_%s%s", def_log_path, pgm_name, def_log_filename, def_name_trailer_txt);  // Default pgm log file name
  sprintf(debuglog_file, "%s%s_%s%s", def_log_path, pgm_name, def_log_filename, def_name_trailer_txt);  // Default pgm's debug log
  
  // Commented out but not forgotten: Non default files - we don't know the names of the input or output files at this time.
  // There is no default.
  // sprintf(in_file    , "%s%s_%s", def_in_path ,     pgm_name, def_in_filename );       // Default pgm's regular log location
  // sprintf(output_file, "%s%s_%s", def_out_path,     pgm_name, def_out_filename);       // Default pgm's regular log location

  // Now, read in any run-time parameters and parse them.

  highest_rc = 0;                         // Set to default, hold the highest return code encountered
  if (pgm_argc == 1)                      // If only the program name found. There will always be at least 1 parm: the program name
  {
    // printf(" No command line (run time) parameters found.\n");
    return 1;                             // That was easy: No further command line parms; return with 1 ('No parameters')
  } 
  else
  {
    /*  Parse each input parameter  */
    param = 1;                           // Intialize to read the first parameter after pgm name
    while ( param < pgm_argc )           // Traverse the chain of parms; make sure you're not going past the end of the array
    {
      rc = extract_param(pgm_argv[param]);    // Work on the next command line parm 
      if (rc != 0)
      {
        if (highest_rc < rc) highest_rc = rc; // Collect the highest return code encountered in this loop
        if (rc > 3)                           // value 4+ - Invalid parameter found.
        {
          // printf("Parameter %d, Unusual: Return code: %d, Parameter: <%s>\n", param, rc, pgm_argv[param]);
          // This may change but right now return;
          return highest_rc;
        }
        else
        {
          // Minor return code. Not enough to halt the program
          // printf("Parameter %d is invalid (ignored): <%s>, Return code: %d.\n", param, pgm_argv[param], rc);
          // 
          ;                                  // Do nothing, continue
        }
      }
      else
      {  ;  }                                // Good parameter nothing more to do
      ++param;                               // Point to the next parameter
    } /* while ( param < pgm_argc ) */
  } // else
  // printf("+++ Done reading runtime parameters ++++\n"); 

  return 0;
} /* get_runtime_parms */

/* ----------------------------------------------------------------------------------
     Get configuration file parameters.
     Like command line parms, these inputs will override the program's internal default
     values that command line parms did not set. 
    
     Also note: All output here is to the command line - no files have yet been opened.
   ---------------------------------------------------------------------------------- */
int  get_config_parms()                          // Get whatever configuration file parameters are present
{
  // Runtime parms set. Now, look for a configuration file.   
  int  input_bufsize   =  500;       // Buffer size for the configuration file record
  char *data_str;                    // Input string buffer (pointer)
  int  line_count      = 0;          // A line count of input records
  int  rc              = 0;          // generic return code for function calls

  /*
  if (cfg_open_set == 'd')                       // If the file name was *not* overridden at runtime
  {
    if (debug_log) printf(" Searching for the default configuration file: %s\n\n", cfg_file);
  }
  else
  {
    if (debug_log) printf(" Searching for configuration file: %s\n\n", cfg_file);
  }
  */
  // Open the defined configuration file
  // If it's not found, BIG problem - return with RC = 4 or higher (will end program run)

  printf(" Searching for configuration file: <%s> ...\n", cfg_file);
  if( (cfg_file_ptr = fopen(cfg_file,"r")) == NULL )        // Try to open the configuration file 
  {
    // Can't find it or won't open 
    // If this file is the default configuration file then simply return, indicate no file found 
    // If it's a specific file (specified through a runtime command parameter), 
    // then return with an error code
    if (cfg_open_set == 'd')                     // If this file was the default file name
    {
      // printf("\nDefault configuration file <%s> not found. Going with existing values.\n", cfg_file);

      return(1);
    }
    else
    {
      // An alternate (non-default) file name was specified but not found, return with an error code
      // The program should halt running in this case.
      printf(" Configuration file <%s> was not found or won't open. Cannot continue.\n", cfg_file);

      return(4);
    }
  }
  
  // File found and opened. Now read through it and get whatever parameter settings are included
  // Be sure to skip all comment lines, defined by a ':' or ';' or '*' in the first column of each line/record

  data_str = (char *) malloc (input_bufsize);     // Allocate an input buffer to read in a record/line
  while (!feof(cfg_file_ptr))
  {
    // ** Assume one switch per line **
    memset(data_str,'\0',input_bufsize);
    fgets(data_str, input_bufsize, cfg_file_ptr);
    line_count++;                                 // Count this line 
    // if (debug_log > 1) printf(" Line %d (before):>>%s<<\n",line_count, data_str);
    rc = trimspaces(data_str);                    // Cleanup: remove leading and trailing spaces, etc...
    // if (debug_log > 1) printf(" Line %d (after):>>%s<<\n",line_count, data_str);
    if (rc == 1)
    {
      // Blank line - no text. To be skipped
      if (debug_log > 2)   printf(" CFG: Line %d skipped - Blank line: <%s>\n",line_count, data_str);
    }
    else
    {
      // Comment lines within the file are flagged as one of the following chars in the first column
      if ( data_str[0] == ':'  ||                 // Look for embedded comment(s)
           data_str[0] == '>'  ||                 // Yes, all these prefixes will denote a comment line
           data_str[0] == '*'  ||                 // because, well, I can
           data_str[0] == ';'  ||
           data_str[0] == ' ' 
         )
      {
        // Comment line - to be skipped
        if (debug_log > 1) printf(" CFG: Line %d comment: <%s>\n",line_count, data_str);
      }
      else
      {
        // Now some configuration file input
        // Assume that there will be only one switch per line
        if (debug_log > 2) printf(" CFG: Line %d: String: <%s>\n",line_count, data_str);
        rc = extract_param(data_str);           // Process the text string found
        if (rc != 0)
        {
          if (rc > 3)                           // Invalid parameter value found.
          {
            printf(" CFG: Parameter <%s>: Unusual return code: %d\n", data_str, rc);

            return rc;                          // Return immediately - bad input 
          }
          else
          {
            // Minor return code. Not enough to halt the program
            // printf("Parameter %d is invalid (ignored): <%s>, Return code: %d.\n", param, pgm_argv[param], rc);
            // 
            if (debug_log > 0) printf(" CFG: Line: %d, String: <%s> is invalid but ignored. Return code: %d.\n", 
                                        line_count, data_str, rc);
            // Do nothing, continue
          }
          // return (rc);
        }
        else
        {  ;  }                                // Good parameter nothing more to do
        ;
      }   // else
    }   // else
  }   // while (!feof(cfg_file_ptr))
  if (debug_log > 1) printf(" CFG: Lines read in %d\n",line_count);   // Display the number of lines read in on this run
  free(data_str);                                // free the memory allocated to read the config file
  fclose(cfg_file_ptr);                          // close the file - no longer needed
  
  return 0;
}  // get_config_parms()

/* -----------------------------------------------------------------------------
* extract_param
*  Parse the parameter/switch string from the text passed to this function.
*  This function is used for command line parameters and configuration file settings.
*  Overrides must be coded to the same rules for both sources.
*
*  In the spirit of massively over-engineering this basic skeleton program....
*
*  This function will overwrite the program's default (hard-coded) values and set 
*  the corresponding flag to indicate an update. This is intended to prevent multiple updates  
*  in the same program run.
*
*  Note: Any resulting error messages will only go to standard output because the log and/or debug files
*        have yet been opened at this (early) stage of program execution.
*
*  Parameters/switches are usually expected to be in the form of 
*      '/x...'  or 
*      '-x...'  or
*      '/x=...' or 
*      '-x=...'
*
*  ie. The '/' or '-' prefix will always be expected, always 1st character
*      The '=' is optional, always the 3rd character  
*
*  ALSO:
*  Check that this parameter string isn't empty (ie. spaces/CRLF after the switch name)
*  and: Ignore LONG parameter strings - longer than what would be considered valid. Treat it as bad input.
*  
*  Note also: Some of the overrides will not be seen in a configuration file but only 
*             as a command line parameter but they're seen here anyways for code simplicity
*
* --------------------------------------------------------------------------------*/
int extract_param(char p_str[])
{
  int rc = 0;                     // Generic rc value for function calls
  int p_str_offset = 0;           // Offset to accomodate varying parameter flags

  if (debug_log> 2) printf(" Ext: String: <%s>\n", p_str);       // Debug: Display the parm you're going to parse

  // Check if this parameter string is long enough to actually potentially hold a value
  // I won't be checking for 'too-long' parameters. Let's assume at this time that this won't happen.
  
  if (strlen(p_str) < 3)                      // if the string is too short to contain a proper switch and value)
  {
    // This parameter string isn't even minimal length to indicate a program switch 
    // and any associated value, so it's invalid. Exit function with a bad rc.
    // if (debug_log > 1) 
    printf(" Ext: Switch <%s> is invalid - too short. Program Exit.\n", p_str);
  
    return(4);                                // Program should stop on this bad input
  }
  else
  {
    if ( (p_str[0] == '-' ) ||                 // Does it begin with a proper switch/parmeter indicator?
         (p_str[0] == '/' )  )
    {
      // Next check if the switch also has a '=' char (3rd position)
      if (p_str[2] == '=')  
      {
        // If it does, then set an offset value be to read the text after this char for the switch text
        p_str_offset = 3;
      }
      else
      {
        // No '=' char found, so set the string offset to 2, to read after the 2nd character of the text
        p_str_offset = 2;
      }
      if (strlen(p_str) > p_str_offset)       // check if there is anything beyond this switch value
      {
        ;                                     // yes, there is a string. We can continue to examine it further down.
      }
      else
      {
        // No, this is an empty switch - return with a bad rc. 
        printf(" Ext: Switch <%s> is empty - no defined setting value. Program Exit.\n", p_str);
  
        return(4);                            // Program should stop on this bad input
      }
    }
    else
    {
        // No, this is parameter does not have a '/' or '-' switch indicator
        if (debug_log > 1) printf(" Ext: Switch <%s> is not a valid parameter. Program exit.\n", p_str);
  
        return(4);                            // Program should stop on this bad input
    }
  }
  /* -------------------------------------------------------------------------
     Parse the individual input string. These parameters may be entered 
     at the command line or configuration file in any order. Badly entered or invalid 
     parameters will be displayed and skipped or flagged as bad input and the program stopped.

     'set' flags are hard-coded to 'd' and will be overwritten to a new value only once.
     If a duplicate/repeated parameter is found, the flag will prevent another update
     and that parmeter skipped.
     ------------------------------------------------------------------------- */

  /* -------------------------------------------------------------------------
     First, check for applicable file names
     ------------------------------------------------------------------------- */
  // Configuration file name - parm '-c' or -C' or '/c' or '/C'
  // -----------------------------------------------------------------------------------
  if ( (p_str[1] == 'c') || (p_str[1] == 'C') )           // If a configuration file name switch
  {
    if (cfg_open_set == 'd')                             // If this value hasn't been updated 
    {
      memset(cfg_file, '\0', sizeof(cfg_file));          // prep - fully NULL out string 
      // If the config file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')    // If there is a qualifying path prefix to the name
      {
        strcat(cfg_file, p_str+p_str_offset);                           // Copy the file name as-is
        if (debug_log > 1) printf(" Ext: Configuration file name: <%s>\n",cfg_file);
      }
      else        
      { 
        strcat(cfg_file, def_path);                                     // Prefix the file name with the default path
        strcat(cfg_file, p_str+p_str_offset);                           // Add the specified file name
        if (debug_log > 1) printf(" Ext: Configuration file name (qualified): <%s>\n",cfg_file);
      }
        cfg_open_set = 'r';                              // Indicate that this default value is now overwritten
    } /* cfg_open_set */
    else                                                 // Already set - ignore
    { 
      if (debug_log > 1) printf(" Ext: Configuration file name already set - Parameter/Configuration setting <%s> skipped.**\n",p_str);
    }
  }  // Configuration file name - if ( (strncmp( p_str, "-c", 2) == 0 ) ...

  else if ( (p_str[1] == 'l') || (p_str[1] == 'L') )      // If a log file name
  {
    if (debug_log > 1) printf(" Ext: Log parm: <%s>\n",p_str);

    
    
    // Add code here to determine if you want or don't want a program log output file

    
    
    if (log_open_set == 'd')                             // If this value hasn't yet been updated
    {
      memset(log_file, '\0', sizeof(log_file));          // prep - init/NULL out string 
      // If the log file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')        // If there is a path prefix to the name
      {
        strcat(log_file, p_str+p_str_offset);                       // Use the file name as-is
        if (debug_log > 1) printf(" Ext: Log file name: <%s>\n",log_file);
      }
      else
      {
        // Otherwise, use the default log path to prefix the log file name
        strcat(log_file, def_log_path);                             // use the default log path prefix
        strcat(log_file, p_str+p_str_offset);                       // add the config file name
        if (debug_log > 1) printf(" Ext: Log file name (qualified): <%s>\n",log_file);
      }
      log_open_set = 'r';                                         // Indicate that this default value is now overwritten
      pgm_log = 1;                                                // Set the flag to produce a program log file
    } /* log_open_set */
    else
    {
      if (debug_log > 1) printf(" Ext: **Log file name already set - Parameter/Configuration setting <%s> skipped.**\n",p_str);
    }
  
  } // if ( (p_str[1] == 'l') || (p_str[1] == 'L') ) // * Log file *
  
  else if ( (p_str[1] == 'v') || (p_str[1] == 'V') )               // If debug/verbose file name
  {
    // Get the debug log file name from the text string
    if (debuglog_open_set == 'd')                                  // If this value hasn't yet been updated
    {
      memset(debuglog_file, '\0', sizeof(debuglog_file));          // prep - NULL fully NULL out string 
      // If the debug log file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')       // If there is a path prefix to the name
      {
        strcat(debuglog_file, p_str+p_str_offset);                 // Use the file name as-is
        if (debug_log > 1) printf(" Ext: Debug/Verbose log file name: <%s>\n",debuglog_file);
      }
      else
      { 
        strcat(debuglog_file, def_log_path);                       // use the default log path prefix
        strcat(debuglog_file, p_str+p_str_offset);                 // add the config file name
        if (debug_log > 1) printf(" Ext: Debug/Verbose log file name (qualified): <%s>\n",debuglog_file);
      }
        debuglog_open_set = 'r';                                   // Indicate that this default value is now overwritten
    } /* debuglog_open_set */
    else
    { 
      if (debug_log > 1) printf(" Ext: **Debug/Verbose file name already set - Override skipped: <%s> skipped.**\n",p_str);
    }
  } //   else if ( (p_str[1] == 'v') || (p_str[1] == 'V') )

  else if ( (p_str[1] == 'o') || (p_str[1] == 'O') )     // If an output data file name
  {
    if (out_open_set == ' ')                             // If this value hasn't yet been specified
    {
      memset(out_file, '\0', sizeof(out_file));          // prep - NULL fully NULL out string 
      // If the program output file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')           // If there is a path prefix to te name
      {
        strcat(out_file, p_str+p_str_offset);            // Use the file name as-is
        if (debug_log > 1) printf(" Ext: Output file name: <%s>\n",out_file);
      }
      else        
      { 
        strcat(out_file, def_out_path);                  // use the default path prefix
        strcat(out_file, p_str+p_str_offset);            // add the config file name
        if (debug_log > 1) printf(" Ext: Output file name (qualified): <%s>\n",out_file);
      }
        out_open_set = 'r';                              // Indicate that this default value is now overwritten
    } /* out_open_set */
    else                                                 // Already set - ignore
    { 
      if (debug_log > 1) printf(" Ext: **Output file name already set - override skipped: <%s>.**\n",p_str);
    }
  }  //   else if ( (p_str[1] == 'o') || (p_str[1] == 'O') ) ...

  else if ( (p_str[1] == 'i') || (p_str[1] == 'I') )      // If an input data file
  {
    if (in_open_set == ' ')                               // If this value hasn't yet been updated
    {
      memset(in_file, '\0', sizeof(in_file));             // prep - fully NULL out string 
      // If the program input file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')     // If there is a path prefix to te name
      {
        strcat(in_file, p_str+p_str_offset);                             // Use the file name as-is
        if (debug_log > 1) printf(" Ext: Input file name: <%s>\n",in_file);
      }
      else        
      { 
        strcat(in_file, def_in_path);                     // use the default path prefix
        strcat(in_file, p_str+p_str_offset);              // add the config file name
        if (debug_log > 1) printf(" Ext: Input file name (qualified): <%s>\n",in_file);
      }
      in_open_set = 'r';                                  // Indicate that this default value is now overwritten
    } //* else if ( (p_str[1] == 'i') || (p_str[1] == 'I') )
    else                                                  // Already set - ignore
    { 
      if (debug_log > 1) printf(" Ext: Input file name previously set - override skipped: <%s>.\n",p_str);
    }
  }  // else if ( (p_str[1] == 'i') || (p_str[1] == 'I') )
  
  else if ( (p_str[1] == 'd') || (p_str[1] == 'D') )
  {
    // Debug flags: Complicated parsing - multiple possibilities for a single switch:
    // Over-engineering:
    // Now we're looking for more multiple setting types, each using the '-D' or '-d' switch
    //
    //     1) Processing debug/verbosity to govern what and how much to print/display
    //        This will set the 'debug_log' global variable
    //        int  debug_log          = 0;      // Create a debug file and set verbosity 
    //                                             ('0'- none/no logging (default),
    //                                              '1'- basic/minimal,
    //                                              '2'- detailed/more,
    //                                              '3'- All details available)
    //                            
    // AND 2) flag for frequent file and output buffer flushes, to help debugging in case the program fails mid-work
    //        This will set the 'debug_flush' global variable
    //        int  debug_flush        = 0;      // Perform frequent debug/log file buffer flushes
    //                                              ('0' - no frequent buffer flush, 
    //                                               '1' - do frequent buffer flushes)

    // First, check for a the 'flush' parameter:
    if ( (p_str[p_str_offset] == 'f') || (p_str[p_str_offset] == 'F') )
    {
      if (debug_flush != 0)                     // If this flag has already been set (ie. not 0) 
      {
        // Output warning message: Flag already set. This setting attempt will be skipped
        if (debug_log > 1)                      // If debug level wants a message
        {
          printf(" Ext: Frequent file flushes already set. This setting <%s> skipped.**\n",p_str);
        }
      }
      else
      {
        // FLag has not yet been set
        debug_flush = 1;                        // Debug: Do frequent output buffer flushes
        if (debug_log > 1) printf(" Ext: Frequent file flushes set: <%s>\n",p_str);
      }
    }
    // Now check if the debug level is being set
    else if ( (p_str[p_str_offset] == 'l') || (p_str[p_str_offset] == 'l') )
    {
      if (debug_log > 0)                        // If debug level has already been set previously (default: 0)
      {
        // Debug level already previously set - ignore this entry
        if (debug_log > 1) printf(" Debug level already set. Override skipped: <%s>.\n",p_str);
        else { ; }
      }
      else
      {
        // The debug level has not been previously set
        // Determine debug level: '0' - no debug logging   (actually the default)
        //                        '1' - basic debug logging
        //                        '2' - more detailed debug logging 
        //                        '3' - All debug details details available
        // Check for a second parameter character. 
        // if there is none, then reject this entry and skip it
        // If wrong or undetermined value, reject and skip
        //
        if (strlen(p_str+p_str_offset) == 2)           // There should be exactly one numbered setting
        {
          // Yes, there is a second parameter character - the debug level is being defined
          if (p_str[p_str_offset+1] == '0')
          {
            debug_log = 0;                   // set the debug output level to 0 - no debug output at all
            if (debug_log > 1) printf(" Ext: Debug: level 0, no debug logging: <%s>\n",p_str);
          }
          else if (p_str[p_str_offset+1] == '1')
          {
            debug_log = 1;                   // set the debug output level to 1 - basic debug output
            if (debug_log > 1) printf(" Ext: Debug: level 1, basic debug data: <%s>\n",p_str);
            debug_log = 1;                   // No debug log output, regardless of whatever other debug settings are made
          }
          else if (p_str[p_str_offset+1] == '2')
          {
            debug_log = 2;                   // set the debug output level to 2 - More detailed debug output  
            if (debug_log > 1) printf(" Ext: Debug: level 2, detailed debug data: <%s>\n",p_str);
          }
          else if (p_str[p_str_offset+1] == '3')
          {
            debug_log = 3;                   // set the debug output level to 2 - More detailed debug output  
            if (debug_log > 1) printf(" Ext: Debug: level 3, *Very* detailed debug data: <%s>\n",p_str);
          }
          else
          {
            if (debug_log > 1) printf(" Ext: Unrecognized debug Level requested - Override skipped: <%s>.\n",p_str);
          }
        }
        else // if (strlen(p_str+p_str_offset) == 2)
        {
          // There was no debug level specified in the parameter
          // Therefore, simply go with the program's default
          {
            debug_log = 0;                   // set the debug output level to 0 - no debug output
            if (debug_log > 1) printf(" Ext: Debug parameter was used but was vague (<%s>). Going with default.\n",p_str);
          }
        }  // else
      } // Else to if (debug_log >1) - If debug level has already been set previously
    }  // else if ( (p_str[p_str_offset] == 'l') ...  Check for debug level
  }  // if ( (strncmp(p_str, "-d", 2) == 0) ...
  // After all this checking, determine that this is an invalid flag/parameter and reject it
  else // final case - flag this is as an invalid parameter
  {
    // Invalid parameter type. It was not prefixed with '-' or '/' 
    // Return with a return code 4
    // This will allow for extracting & validating further parameters
    // Highlight the bad parm and ignore/skip 
    if (debug_log > 1) printf(" Ext: Parameter <%s> is invalid (ignored).\n", p_str);
  
    return(4);
  } /* if ( (strncmp( p_str, "-", 1) == 0 )  ||...  */ 

  return  rc;
} /* extract_param */

/* ----------------------------------------------------------------------------- */
int trimspaces(char *data_str_ptr)
{
#define NEWLINE 0x0a
#define CARRIAGE_RETURN 0x0d

  /* This function trims all leading spaces and trailing spaces, newlines, funny chars,
     carriage returns from the string
  */
 
  int i,sl,startpos;
  char tmp_str[30000];            // assume maximum string length is this

  // Find the start position of the string (first character)
  // Start at the beginning and work up to the first Dispaly character
  startpos = -1;
  sl = strlen(data_str_ptr);
  i = 0;
  while (i < sl) {
    if ( (data_str_ptr[i]==' ')  ||
         (data_str_ptr[i]==0x0)  ||      /* Binary 0x00 - ^@       */
         (data_str_ptr[i]==0x1)  ||      /* Binary 0x01 -          */
         (data_str_ptr[i]==0x2)  ||      /* Binary 0x02 -          */
         (data_str_ptr[i]==0x3)  ||      /* Binary 0x03 -          */

         (data_str_ptr[i]==0x4)  ||      /* Binary 0x04 -          */
         (data_str_ptr[i]==0x5)  ||      /* Binary 0x05 -          */
         (data_str_ptr[i]==0x6)  ||      /* Binary 0x06 -          */
         (data_str_ptr[i]==0x7)  ||      /* Binary 0x07 - ^G       */
         (data_str_ptr[i]==0x9)  ||      /* Binary 0x09 - tab char */

         (data_str_ptr[i]==0xc)  ||      /* Binary 0xc  - ^L       */
         (data_str_ptr[i]==0xe)  ||      /* Binary 0xf  - ^E char  */
         (data_str_ptr[i]==0xf)  ||      /* Binary 0xf  - ^O char  */
         (data_str_ptr[i]==0x10) ||      /* Binary 0x10  -         */
         (data_str_ptr[i]==0x14) ||      /* Binary 0x14 -          */

         (data_str_ptr[i]==0x15) ||      /* Binary 0x15 -          */
         (data_str_ptr[i]==0x16) ||      /* Binary 0x15 - ^V char  */
         (data_str_ptr[i]==0x1a) ||      /* Binary 0x1a -          */
         (data_str_ptr[i]==0x1b) )       /* Binary 0x1b - ^[       */
    {
      i++;                  // Look at the next character down the string
    }
    else {
      startpos = i;         // Remember the position of the first character found
      i=sl;
    }
  }
  if (startpos == -1) 
  {
    /* Nothing but blanks - set to NULL. This is a blank line */
    data_str_ptr[0]='\0';

    return(1);
  }
  
  // Next start at the end of the string and work backwards, Nulling out any non-display characters
  // Until we get to the end of the readable input string
  sl = strlen(data_str_ptr) - 1;
  for (i=sl; i>=startpos; i--)
  {
    if (  (data_str_ptr[i]!=' ')      &&
          (data_str_ptr[i]!=0x0)      &&
          (data_str_ptr[i]!=0x1)      &&
          (data_str_ptr[i]!=0x2)      && 
          (data_str_ptr[i]!=0x3)      && 
          (data_str_ptr[i]!=0x4)      && 
          (data_str_ptr[i]!=0x5)      && 
          (data_str_ptr[i]!=0x6)      && 
          (data_str_ptr[i]!=0x7)      && 
          (data_str_ptr[i]!=0x9)      && 
          
          (data_str_ptr[i]!=0xc)      && 
          (data_str_ptr[i]!=0xe)      && 
          (data_str_ptr[i]!=0xf)      && 
          (data_str_ptr[i]!=0x10)     && 
          (data_str_ptr[i]!=0x14)     && 
          (data_str_ptr[i]!=0x15)     && 
          (data_str_ptr[i]!=0x16)     && 
          (data_str_ptr[i]!=0x1a)     && 
          (data_str_ptr[i]!=0x1b)     && 
          (data_str_ptr[i]!='\n')     && 
          
          (data_str_ptr[i]!=NEWLINE)  && 
          (data_str_ptr[i]!=CARRIAGE_RETURN)
       )
     {
      // Found the end of the displayable string. Return only this (displayable) part of the string.
      data_str_ptr[i+1]='\0';
      memset(tmp_str , '\0', sizeof(tmp_str)   );
      strcpy(tmp_str , &data_str_ptr[startpos] );
      sl = strlen(data_str_ptr);
      memset(data_str_ptr, '\0', sl);                   // done this way to avoid a mysterious compiler warning
      strcpy(data_str_ptr, tmp_str);                    // Overwrite the incoming string with the reduced string

      return(0);
    }
    // 
  
  
  }
  /* Nothing but blank/new line/carriage return -- set the incoming string to null */
  data_str_ptr[0]='\0';

  return(1);                                            // Indicate a blank line
} /*-- trimspaces */