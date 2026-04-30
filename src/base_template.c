/* ----------------------------------------------------------------------------------------------------------------------
  base_template.c
    Author: Edward Morawski

    This program is a C Language base template for future programs written my me.
    It does some initial startup stuff that I like to have in my programs, such as:
    - Display some program startup information at run time
    - Open any input and output files, if any
    - Set basic flags and text strings
    - Any other basic setup and output that may be a good idea to have
    
    As such, this is a runnable skeleton program that's meant to be a basis for some real stuff.
    This is a 'live' file and will be modified as needed.

    Note: Before you say anything about the code style/practices:
        1) Yes, some of the following is 'old school' practice. It's my style. 
        2) There are deliberately a LOT of comments. It's for code clarity and to remind myself months+ later
        3) Since this is generic skeleton code for future programs:
          a) There is code for both basic input and output files if needed.
          b) There are NO default input and output file names defined. If you need to use input and/or output file(s),
             names absolutely need to be added as parameters.
   ----------------------------------------------------------------------------------------------------------------------*/

// Turn off the MS-oriented secure function call warnings. They are considered near-useless anyway
// #define  _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>                         // C Standard library
#include <stdio.h>                          // Standard I/O functions
#include <string.h>                         // String manipulation
#include <time.h>                           // Times and time strings
#include <ctype.h>                          // For detecting specific character types

/* ----------------------------------------------------------------------------------------- 
   Default file names. May be updated via runtime parameter(s) or a config file.
  ----------------------------------------------------------------------------------------- */
char def_cfg_filename     [] = "cfg.txt\0";         // Default run-time configuration file name
char def_log_filename     [] = "log.txt\0";         // Default output log  name - 'just the essentials'
char def_debuglog_filename[] = "verbose_log.txt\0"; // Default output file name for verbose log and debug output
char def_in_filename      [] = "input.txt\0";       // Default input data file
char def_out_filename     [] = "output.txt\0";      // Default output data file

// Default paths - the expected *relative* locations of pre-coded files:
// By default it's assumed that the program is being run from its 'bin' directory
char def_path             [] = "./\0";              // Default generic path for files
char def_log_path         [] = "../log/\0";         // Default path - output log(s)
char def_debuglog_path    [] = "../log/\0";         // Default path - verbose processing log file
char def_in_path          [] = "../data/\0";        // Default path - input data file(s)
char def_out_path         [] = "../output/\0";      // Default path - output file(s)

// program name as derived at run-time.
char pgm_name       [40];         // Program name minus leading non-alpha chars

// Files (in their final form) to work with 
char cfg_file      [100];         // Configuration file name
char log_file      [100];         // Log output file name
char debuglog_file [100];         // Verbose/Debug log output file name
char in_file       [100];         // Program input file name
char out_file      [100];         // Program output file name

// Flags to indicate if the above files are actually opened. Default value is 0 (ie. 'no')
int  cfg_open      = 0;
int  log_open      = 0;
int  debuglog_open = 0;
int  in_open       = 0;
int  out_open      = 0;

// Flags to indicate where file names were set. This is important at startup
// Show that it was set and where. This will help enforce the practice that it is set only once.
// Valid values: ' ' - Not set at all
//               'd' - default
//               'r' - set through run-time parameter
//               'c' - set through configuration file setting
//               ' ' - no default setting 
char cfg_open_set       = 'd';    // Flag: Indicate the configuration file name is set
char log_open_set       = 'd';    // Flag: Indicate standard program output log name is set
char debuglog_open_set  = 'd';    // Flag: Indicate the debug file name is set
char in_open_set        = ' ';    // Flag: Indicate the data input file name is set - not pre-defined by default
char out_open_set       = ' ';    // Flag: Indicate the data output file name is set - not pre-defined by default

/* Debugging flags with starting default values (Default - 0 (ie. 'no'/'none')*/
int  debug_flush        = 0;      // Flag: Perform frequent debug/log file buffer flushes (0 - no-rapid flushes, 1 - frequent buffer flushes)
int  debug_log          = 0;      // Flag: Create a debug file and set its verbosity (1- basic, 2- detailed, 3- All details available)

/* Time/date values in a couple of formats.
   Note: These fields/values are defined globally in case they are used beyond the main() function.
   Hint: You'll find some alternative time/date ideas from the personal program timeDate.c  
*/
time_t  system_Time;              // Local copy of the system raw time at program start
struct  tm loc_start_Time;        // Local formatted copy of the derived local time
struct  tm UTC_start_Time;        // Local formatted copy of the derived GMT/UTC time structure
char    loc_Time_str   [25];      // char version of loc_Time
char    UTC_Time_str   [25];      // char version of UTC_Time
char    gen_datetimestr[50];      // Char version - tmp date/time string, formatted and used as needed

char    loc_start_Time_str[25];   // local date & time string
char    UTC_start_Time_str[25];   // GMT/UTC date & time string
char    gen_datetimestr   [50];   // generic date/time string - tmp/working string

/* Function profiles */
int get_runtime_parms(int, char * []);    // Read the runtime parms
int extract_param     (int, char  []);    // Extract and edit an individual parameter from a string
int get_parm         (int, char * []);    // Parse a command-line parameter

/*------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  int   ret  = 0;                         // Generic return code value
  char *cptr = NULL;                      // pointer to a char, actually a pointer to an embedded string

  // --------------------------------------------------------------------------------------------------------------------------------
  // Initialize time/date fields from system calls
  // --------------------------------------------------------------------------------------------------------------------------------

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

  strncpy(loc_start_Time_str, asctime(&loc_start_Time), 24);    // copy basic local date/time string, leave out the trailing '\n'
  strncpy(UTC_start_Time_str, asctime(&UTC_start_Time), 24);    // copy basic GMT   date/time string, leave out the trailing '\n'

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
  
  // Now display some startup information, This is to show that the program is actually running.
  printf("\n\n");                                                    // Separate output from any preceeding terminal text 
  printf("Program: %s\n", pgm_name);                                 // Display this program's name
  printf("Program Compile Date/Time: %s/%s\n", __DATE__, __TIME__);  // Program's compile date and time
  printf("Current time: (local): %s, ", loc_start_Time_str);         // 'local time' string
  printf("(UTC): %s", UTC_start_Time_str);                           // UTC time to the string  
  printf("\n\n");                                                    // Finish line with line feed(s) 

  /* -------------------------------------------------------------------------------------------------
    Get all runtime parameters and then configuration file parameters to set the overall environment 
    Default settings are hard-coded and are overwritten this way or through the configuration file settings.
    Process for setting defaults:
      1) Program already has hard-coded default values, to be used if no override is found
      2) Command line values override hard-coded values
      3) Configuration file settings override default values not already set by the command line parms 
     -------------------------------------------------------------------------------------------------*/
 
  ret = 0;                                          // Generic return code var
  ret = get_runtime_parms(argc, argv);              // Get whatever runtime/command line parameters are present
  if (ret == 1)                                     // If no runtime command line parameters included
  {
    ;                                               // Ignore - no command line parameters found
    // printf("*** No command line parameters. \n");   // Indicate that there were no runtime parms found.
  }
  else
    if (ret > 3)                                    // Problem with runtime command line parm(s) - ending program now.
    {
      printf("**** Bad/Invalid command line input - ending now. ****\n");

      return ret;
    }
    else
    {
      ;                                             // Do nothing - it all worked
    }

  // Runtime parms set. Now, look for a configuration file.   
  printf("Searching for configuration file: %s\n\n", cfg_file);

  /* --------------   Do the config file thing... ----------------- */
  /* Look for the configuration file for this program run.
  * Use the run-time parameter for this, if it is present. Otherwise use the default name.
  * If a configuration file is found, open it and process its contents for final configuration 
  * If there is no configuration file to be found, use any default values present.
  */





  /* --------------   Do the log file thing... ----------------- */
  /*
     Open the program's log file using the resolved file name, if there is one.
     Open the program's detailed log file, if wanted, also using the resolved file name.
     Set any other runtime input parameters.
     Note: pgm_name is already set (above)...
  */




  
  /* --------------   Do more config stuff... ----------------- */
  /* Now get the specific processing environment(s) vars that this program needs to work in,
     such as MariaDB, MYSql, etc... database vars, required GUI-based environmental settings, etc.
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

  sprintf(cfg_file,      "%s%s_%s", def_path,          pgm_name, def_cfg_filename);       // Set default config file name
  sprintf(log_file,      "%s%s_%s", def_log_path,      pgm_name, def_log_filename);       // Set default pgm log file location
  sprintf(debuglog_file, "%s%s_%s", def_debuglog_path, pgm_name, def_debuglog_filename);  // Set default pgm's verbose log location
  
  // Commented out but not forgotten: Non default files - we don't know the names of the input or output files at this time.
  // And may never need them. This depends on the purpose of the program.
  // sprintf(in_file    , "%s%s_%s", def_in_path ,     pgm_name, def_in_filename );       // Default pgm's regular log location
  // sprintf(output_file, "%s%s_%s", def_out_path,     pgm_name, def_out_filename);       // Default pgm's regular log location

  // Now read in any run-time parameters and parse them.

  // printf("Program parameters (strings) at run time:\n");
  // printf("Parameter count after program name: %d\n", pgm_argc-1);

  ret_rc = 0;                             // Set to default, hold the highest return code encountered
  if (pgm_argc == 1)                      // If only the program name found. There will always be at least 1 parm: the program name
  {
    // printf("*** No command line (run time) parameters found ***\n");
    return 1;                             // That was easy: No further command line parms; return with 1 ('No parameters')
  } 
  else
  {
    /* Parse out the runtime parameters */
    if (pgm_argc > 12)                     // Too many parameters - don't even try - return with error 16
    {
      // Too many parms - include this to flag a mistake such as an improperly set long string value
      printf("*** Too many run-time parameters (%d of them!). Not even trying... ***\n", pgm_argc-1);

      return 16;                          // Return with 16 - too many parameters
    }
    else
    {
      printf("Number of parameters: %d\n", pgm_argc -1);     // Display the number of parameters, excluding parm 0 (program name)
      /*  Parse each input parameter  */
      param = 1;                           // Intialize to read the first parameter after pgm name
      while ( param < pgm_argc )           // Traverse the chain of parms; make sure you're not going past the end of the array
      {
        rc = extract_param(param,  pgm_argv[param]);   // Function: Get the next command line parm 
        if (rc != 0)
        {
          if (ret_rc < rc) ret_rc = rc;         // Collect the highest return code encountered in this loop
          if (rc > 4)                           // Invalid parameter found.
          {
            // printf("Parameter %d, Unusual: Return code: %d, Parameter: <%s>\n", param, rc, pgm_argv[param]);
            // This may change but right now return;
            return ret_rc;
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
    }  // else
  } // else
 
  printf("+++ Done reading runtime parameters ++++\n"); 

  return 0;
} /* get_runtime_parms */

/* -----------------------------------------------------------------------------
* extract_param
*  Extract and parse the parameter string from the text passed to this function.
*  This function will overwrite the program's default (hard-coded) values and set 
*  the appropriate flag to indicate an update. This update process is intended 
*  to prevent serial updates, regardless of source.
*  This function is intended to be called for both command line parameters and 
*  configuration file input.
*
*  Note: The error messages will only go to standard output because the log and 
*        debug files (if any) have not yet been opened at this stage of program startup.
*
*  Since we're being anal and massively over-engineering this basic skeleton program....
*
*  Parameters/switches are usually expected to be in the form of '/x...' or '-x...'
*  (ie. single character and maybe also with a trailing '=' (ie, '/x=' or '-x=')
*  followed by the intended value for that switch
*
*  So check if the parameter includes a '=' character in the third space (ie. '-c=')
*  If it does, then the parameter value parsing needs to accomodate this extra character
*
*  ALSO:
*  Check that this parameter string isn't empty (ignore LONG parameter strings for now; 
*  they would be invalid anyways)
*  
* --------------------------------------------------------------------------------*/
int extract_param(int p_num, char p_str[])
{
  int rc = 0;
  int p_str_offset = 0;          // Offset to accomodate varying parameter flags

  printf(" Parameter %d: <%s>", p_num, p_str);       // Debug: Display the parm you're going to parse

  // Check if this parameter string is long enough to actually potentially hold a value
  // I won't be checking for 'too-long' parameters. Let's assume at this time that this won't happen.
  
  if (strlen(p_str) > 2)                      // if the parameter string is a proper minimum string length (without a '=' char)
  {
     if ( (p_str[0] == '-' ) ||                // Does it begin with a proper parmeter indicator?
          (p_str[0] == '/' )  )
    {
      // Next check if the switch has a '=' char
      if (p_str[2] == '=')  
      {
        // If it does, then be sure to read the text after this char
        // This means setting an offset value to read the text after this '=' character
        p_str_offset = 3;
      }
      else
      {
        // No '=' char found, so set the string offset to 2, to read simply after this switch value
        p_str_offset = 2;
      }
      if (strlen(p_str) > p_str_offset)       // check if there is anything beyond this switch value
      {
        ;                                     // yes, there is a string. We can continue to examine it.
      }
      else
      {
        // No, this is an empty switch
        printf(" Parameter %d is skipped - no value defined for this parameter: <%s>.\n", p_num, p_str);
  
        return(4);
      }
    }
    else
    {
        // No, this is parameter does not have a '/' or '-' switch indicator
        printf(" Parameter %d is skipped - invalid parameter: <%s>.\n", p_num, p_str);
  
        return(4);
    }
  }
  else
  {
    // This parameter string isn't even minimal length to indicate a program switch 
    // and any associated value, so it's invalid
    printf(" Parameter %d is skipped - invalid/too short): <%s>.\n", p_num, p_str);
  
    return(4);
  }
  /* -------------------------------------------------------------------------
     Parse out the individual program parameter. These parms may be entered 
     at the command line in any order. Badly entered or invalid parms will be
     flagged/displayed and skipped.

     'set' flags are hard-coded to 'd' and will be overwritten to a new value only once.
     If a duplicate/repeated parameter is found, the flag will prevent another update
     and that parmeter skipped.
     ------------------------------------------------------------------------- */

  /* -------------------------------------------------------------------------
     First, check for applicable file names sets...
     ------------------------------------------------------------------------- */
  // Configuration file name - parm '-c' or -C' or '/c' or '/C'
  // Logicaly, this is the only place you'll find an alternative config file name 
  // specified because, well, it's the configuiration file name that you're looking for. 
  // -----------------------------------------------------------------------------------
  if ( (p_str[1] == 'c') || (p_str[1] == 'C') )
  {
    // Look for a run-time configuration file name parameter
    if (cfg_open_set == 'd')                             // If this value is the default - not updated
    {
      memset(cfg_file, '\0', sizeof(cfg_file));          // prep - fully NULL out string 
      // If the config file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')    // If there is a qualifying path prefix to the name
      {
        strcat(cfg_file, p_str+p_str_offset);                           // Copy the file name as-is
        printf(" Configuration file name: <%s>\n",cfg_file);
      }
      else        
      { 
        strcat(cfg_file, def_path);                                     // Prefix the file name with the default path
        strcat(cfg_file, p_str+p_str_offset);                           // Add the specified file name
        printf(" Configuration file name (qualified): <%s>\n",cfg_file);
      }
        cfg_open_set = 'r';                              // Indicate that this default value is now overwritten
    } /* cfg_open_set */
    else                                                 // Already set - ignore
    { 
      printf(" **Configuration file name already set - Parameter/Configuration setting '%s' skipped.**\n",p_str);
    }
  }  // Configuration file name - if ( (strncmp( p_str, "-c", 2) == 0 ) ...
  
  // Define a program processing log file name - parameter '-l=' or '-L=' ..
  else if ( (p_str[1] == 'l') || (p_str[1] == 'L') )
  {
    if (log_open_set == 'd')                             // If this value hasn't yet been specified
    {
      memset(log_file, '\0', sizeof(log_file));          // prep - NULL fully NULL out string 
      // If the log file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')           // If there is a path prefix to the name
      {
        strcat(log_file, p_str+p_str_offset);                       // Use the file name as-is
        printf(" Processing log file name: <%s>\n",log_file);
      }
      else
      {
        strcat(log_file, def_log_path);                             // use the default log path prefix
        strcat(log_file, p_str+p_str_offset);                       // add the config file name
        printf(" Processing log file name (qualified): <%s>\n",log_file);
      }
        log_open_set = 'r';                                         // Indicate that this default value is now overwritten
    } /* log_open_set */
    else
    {
      printf(" **Log file name already set - Parameter/Configuration setting '%s' skipped.**\n",p_str);
    }
  } // if ( (p_str[1] == 'l') || (p_str[1] == 'L') )
  
  // Define a verbose/debug file name - parameter '-v=' or '-V='..
  else if ( (p_str[1] == 'v') || (p_str[1] == 'V') )
  {
    if (debuglog_open_set == 'd')                                          // If this value hasn't yet been specified
    {
      memset(debuglog_file, '\0', sizeof(debuglog_file));                  // prep - NULL fully NULL out string 
      // If the debug log file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')       // If there is a path prefix to te name
      {
        strcat(debuglog_file, p_str+p_str_offset);                  // Use the file name as-is
        printf(" Debug/Verbose log file name: <%s>\n",debuglog_file);
      }
      else        
      { 
        strcat(debuglog_file, def_log_path);                        // use the default log path prefix
        strcat(debuglog_file, p_str+p_str_offset);                  // add the config file name
        printf(" Debug/Verbose log file name (qualified): <%s>\n",debuglog_file);
      }
        debuglog_open_set = 'r';                                    // Indicate that this default value is now overwritten
    } /* debuglog_open_set */
    else
    { 
      printf(" **Debug/Verbose file name already set - Parameter/Configuration setting '%s' skipped.**\n",p_str);
    }
  } //   else if ( (p_str[1] == 'v') || (p_str[1] == 'V') )

  // Define a data output file name
  else if ( (p_str[1] == 'o') || (p_str[1] == 'O') )
  {
    if (out_open_set == ' ')                             // If this value hasn't yet been specified
    {
      memset(out_file, '\0', sizeof(out_file));          // prep - NULL fully NULL out string 
      // If the program ouput file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')           // If there is a path prefix to te name
      {
        strcat(out_file, p_str+p_str_offset);            // Use the file name as-is
        printf(" Output file name: <%s>\n",out_file);
      }
      else        
      { 
        strcat(out_file, def_out_path);                  // use the default path prefix
        strcat(out_file, p_str+p_str_offset);            // add the config file name
        printf(" Output file name (qualified): <%s>\n",out_file);
      }
        out_open_set = 'r';                              // Indicate that this default value is now overwritten
    } /* out_open_set */
    else                                                 // Already set - ignore
    { 
      printf(" **Output file name already set - Parameter/Configuration setting '%s' skipped.**\n",p_str);
    }
  }  //   else if ( (p_str[1] == 'o') || (p_str[1] == 'O') ) ...

  // Define an input file name
  else if ( (p_str[1] == 'i') || (p_str[1] == 'I') )
  {
    if (in_open_set == ' ')                              // If this value hasn't yet been specified
    {
      memset(in_file, '\0', sizeof(in_file));            // prep - fully NULL out string 
      // If the program input file name is fully qualified (ie. with a path prefix) then use it as-is,
      // else prefix the file name with the default qualifying directory
      if (p_str[p_str_offset] == '.' ||  p_str[p_str_offset] == '/')     // If there is a path prefix to te name
      {
        strcat(in_file, p_str+p_str_offset);                             // Use the file name as-is
        printf(" Input file name: <%s>\n",in_file);
      }
      else        
      { 
        strcat(in_file, def_in_path);                                     // use the default path prefix
        strcat(in_file, p_str+p_str_offset);                              // add the config file name
        printf(" Input file name (qualified): <%s>\n",in_file);
      }
        in_open_set = 'r';                               // Indicate that this default value is now overwritten
    } //* else if ( (p_str[1] == 'i') || (p_str[1] == 'I') )
    else                                                 // Already set - ignore
    { 
      printf(" **Input file name previously set - Parameter/Configuration setting '%s' skipped.**\n",p_str);
    }
  }  // else if ( (p_str[1] == 'i') || (p_str[1] == 'I') )
  
  /* Processing debug/verbosity to govern what and how much to print/display */ 
  else if ( (p_str[1] == 'd') || (p_str[1] == 'D') )
  {
    // Here we're looking at three possible settings, all using the '-D' switch
    // So '-D'...  can be used more than once but each specific setting should be done only once
    if ( (strncmp(p_str, "-d", 2) == 0) || (strncmp(p_str, "-D", 2) == 0) )   
    {
        // Determine the debug level being requested
        /* Debugging flags with starting default values (Default - 0 (ie. 'no')
           int  debug_flush        = 0;      // Perform frequent debug/log file buffer flushes
           int  debug_log          = 0;      // Create a debug file and set verbosity ('1'- basic, '2'- detailed, '3'- All details available)
        */
        if ( (p_str[p_str_offset] == 'b') || (p_str[p_str_offset] == 'B') )
        {
          if (debug_flush != 0)      // If this flag has already been set
          {
            // Outout warning message: Flag already set and this one ignored
            printf(" **Frequent debug file flushes already set. Parameter/configuration setting '%s' skipped.**\n",p_str);
          }
          else
          {
            debug_flush = 1;                     // Debug: Do frequent output buffer flushes
            printf(" **Debug: Frequent debug file flushes set: '%s'\n",p_str);
          }
        }
        else if ( (p_str[p_str_offset] == 'l') || (p_str[p_str_offset] == 'L') )      // Debug level setting
        {  
          if (debug_log != 0)
          {
            printf(" **Debug level already set. Parameter/configuration setting '%s' skipped.**\n",p_str);
          }
          else
          {
            // Determine debug level: '1' - basic,
            //                        '2' - more detailed, 
            //                        '3' - All details available
            //Check for a second parameter character. 
            // If there is none, then reject and skip this
            // Else determine the debug level specified
            // If wrong or undetermined value, reject and skip
            if (strlen(p_str+p_str_offset) > 1)
            {
              // Yes, there is a second parameter character - the debug level being requested
              if (p_str[p_str_offset+1] == '1')
              {
                debug_log = 1;                   // set the debug output level to 1 - basic debug output
                printf(" **Debug: Debug level 1, basic debug data: '%s'\n",p_str);
              }
              else if (p_str[p_str_offset+1] == '2')
              {
                debug_log = 2;                   // set the debug output level to 2 - More detailed debug output  
                printf(" **Debug: Debug level 2, detailed debug data: '%s'\n",p_str);
              }
              else if (p_str[p_str_offset+1] == '3')
              {
                debug_log = 3;                   // set the debug output level to 2 - More detailed debug output  
                printf(" **Debug: Debug level 3, *Very* detailed debug data: '%s'\n",p_str);
              }
              else
                printf(" **Unrecognized Debug Level requested - Parameter/configuration setting '%s' skipped.**\n",p_str);
            }  // if (strlen(p_str+p_str_offset) > 1)
          }  // else
        }  // else if ( (p_str[p_str_offset] == 'l') ...
    }  // if ( (strncmp(p_str, "-d", 2) == 0) ...
  }  // if ( (strncmp( p_str, "-o=", 2) == 0 ) ...

  // After all this checking, determine that this is an invalid flag/parameter and reject it
  else // final case - (no 'if'...) flag this is definitely an invalid parameter
  {
    // Invalid parameter type. It was not prefixed with '-' or '/' 
    // Return with a return code 4
    // This will allow for extracting & validating further parameters
    // Highlight the bad parm and ignore/skip 
    printf(" Parameter %d is invalid (ignored): <%s>.\n", p_num, p_str);
  
    return(4);
  } /* if ( (strncmp( p_str, "-", 1) == 0 )  ||...  */ 

  return  rc;
} /* extract_param */