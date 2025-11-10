/*  base_template.c
    This program is a base template for future programs written my me.
    It is a basic template that does some initial startup stuff that I 
    like to have in programs, such as:
    - Program information at run time
    - input and output files, if any
    - startup parameters from the input line of configuration file, if any
    - any other basic output that I might want up front
*/

#include <stdlib.h>                         // C Standard library
#include <stdio.h>                          // Standard I/O functions
#include <string.h>                         // String manipulation
#include <time.h>                           // Times and time strings
#include <ctype.h>                          // For detecting specific character types

// #include "timeDate.h"


/* Control and processing values (Shamelessly copied from some programs
   I had produced while still gainfully employed so many years ago.               */

/* Buffers and vars that may be referenced throughout this listing                */
/* Some basic input and control buffers, vars, etc. for this program.             */
/* Default values - in case the program is run without input partms or config values */
char def_cfg_filename    [] = "./template_config.txt\0"; // Default pgm configuration input file name
char def_log_filename    [] = "./logfile.txt\0";         // Default output log file name
char def_det_filename    [] = "./detail_log.txt\0";      // Default detailed log output file name
char def_output_filename [] = "./output.txt\0";          // Default program standard processing output file name

/* Set at runtime, either withthe default values (above) or runtime input parms */
char pgm_name[40];                                    // Program name - This program, minus leading non-alpha chars
char cfg_input          [100];                        // Program configuration file name
char log_file           [100];                        // Program log output file
char log_detail         [100];                        // Program detailed log output
char output_file        [100];                        // Program standard output



/*
char todaydate[7];                            // System date
char input_file_name[100];                    // Principle input file to this program
char full_input_file_name[100];               // Fully qualified input file name
char log_file_name[100];                      // This program's output log file name
char full_log_file[100];                      // Fully qualified output log file name
char debug_file_name[100];                    // This program's debug info file name
char full_debug_file[100];                    // Fully qualified debug info file name

char input_file_basename[100];                // 1st part of input file name
char input_file_findtype[100];                // Which FIND file type - from basename
char input_file_findtype_u[100];              // FIND file type - in UPPER CASE
*/

/* Any connection vars needed for a database connection */
// char *userid;
// char *conn_name;
// char db_env[26];

/* Time buffers to determine local time */
// struct tm *t_ptr_sys;                      // Time struct to hold the local system time
// time_t sys_t;                              // variable to store the local system time
// char now_time_date_loc[26],                // Store the current local system time &  date
// now_time_date_utc[26];                     // store local time&date in GMT time

// More, formatted time vars for display/store/ etc based on the system time sys_t
// char rundate[26];
// char current_date[12];                     // Oracle-compatable date 
// char current_date6[7];                     // Short-form date: yymmdd
// char nowdate[26];                          // today's date 
// char tmp_mnth[3];                          // today's month (abbreviated)




/* Function profiles */
int Get_runtime_parms(int, char *[]);


/*------------------------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
  int i   = 0;                                             // Generic increment counter
  int ret = 0;                                             // Generic return code reader

  /* Hint: Get some alternative time/date ideas from the program timeDate.c  */
  time_t   system_Time = time(NULL);                       // define and init a local copy of the system raw time 
  struct  tm loc_Time  = *localtime(&system_Time);         // define and init a local copy of the derived local time
  struct  tm UTC_Time  = *gmtime(&system_Time);            // define and init a local copy of the derived GMT/UTC time structure
  char loc_Time_str[25];                                   // local date & time string (used later)
  char UTC_Time_str[25];                                   // GMT/UTC date & time string (used later)
  char gen_datetimestr[50];                                // generic date/time string
  
  memset(loc_Time_str,    '\0', sizeof(loc_Time_str));     // initialize first 
  memset(UTC_Time_str,    '\0', sizeof(UTC_Time_str));     // initialize first
  memset(gen_datetimestr, '\0', sizeof(gen_datetimestr));  // initialize first

  strncpy(loc_Time_str, asctime(&loc_Time), 24);           // create basic local time date/time string, leave out the trailing '\n'
  strncpy(UTC_Time_str, asctime(&UTC_Time), 24);           // create basic GMT date/time string, leave out the trailing '\n'

  /* First, some basic program info, because it's available and I can get it */
  memset(pgm_name, '\0', sizeof(pgm_name));                // Initialize, in case of initially dirty buffer
  
  /* Get the runtime program name from the argument list and strip off the leading navigation/directory chars */
  i = 0;                                                   
  while ( isalnum(argv[0][i] ) == 0)                       // Detect non-alphanumeric char, skip it if found
    i++;
  strcpy(pgm_name, argv[0]+i);                             // Keep the resulting program name
    
  printf("\nProgram: %s\n", pgm_name);                    // Display this program's name
  printf("Program Compile Date/Time: %s/%s\n\n", __DATE__, __TIME__);  // Display this program's compile date and time

  printf("Current time: (local): %s, ", loc_Time_str);    // Print out the 'local time' string
  printf("(UTC): %s", UTC_Time_str);                      // include UTC time to the string 
  printf("\n\n");                                          // finish line with line feed(s) 

  /* Set default runtime values. These *may* be overridden with runtime program parms or input config file info */
  // Note: pgm_name is already set (above)...

  /* First initialize everything to NULL */
  memset(cfg_input,   '\0', sizeof(cfg_input));    
  memset(log_file,    '\0', sizeof(log_file));    
  memset(log_detail,  '\0', sizeof(log_detail));    
  memset(output_file, '\0', sizeof(output_file));    
 
  /* Copy into the file names list the default values. These MAY be overridden with config or runtime parms */
  strcpy(cfg_input,   def_cfg_filename);
  strcpy(log_file,    def_log_filename);
  strcpy(log_detail,  def_det_filename);
  strcpy(output_file, def_output_filename);    

  /* Get any further run-time parameters */
  ret = 0;
  ret = Get_runtime_parms(argc, argv);
  if (ret == 1)                         // No runtime command line parameters included
  {
    printf("*** No runtime command line parameters. \n");
  }
  else
    if (ret == 4)                       // Problem with runtime command line parm(s) - ending program now.
    {
      printf("*** Invalid runtime command line parm - ending program run now.\n");

      return ret;
    }


  /* Now get the specific processing environment(s) vars that this program needs to work in,
     such as
     MariaDB, MYSql, etc... database vars, required GUI-based environmental settings, etc.
     But, don't forget the immediate stuff such as system time and date, file names, etc....
  */

  return ret;
}

int Get_runtime_parms(int pgm_argc, char *pgm_argv[])
{
  /*
  Get and parse the program's runtime parameters.
  Get the configuration file's parameters
  Resolve them with the default values.
  Open the program's log file using the resolved file name.
  Open the program's detailed log file, if wanted, also using the resolved file name.
  Set any  other runtime input parameters.
  */

  /*
  First read in any run-time parameters and parse them.
  */
    int param = 1;            // .start with the 2nd parm - after the program name - '0'

    // printf("Program parameters (strings) at run time:\n");

    // Simple alternative: display the first character of each parameter
    // printf("Input parms - first character of each parm at run time:\n");
    printf("Program command line parameter count: %d\n", pgm_argc);
    /*
      If too many parameters, then end an error message and end immediately 
    */
    if (pgm_argc > 6)
    {
      printf("*** Parameter count: %d, Too many runtime parameters. ***\n", pgm_argc);

      return 4;
    }




    param = 1;
    while (param < pgm_argc)
    {
      printf("Parameter %d is %s\n", param, pgm_argv[param]);
      /* Parse the input parm */












      param++;
    }

  if (param > 1)
    return 0;
  else
    return 1;
}
