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

/* Default values - in case this program is run without input parms or a config file 
   that include one or more of the following 'def_' values.
   All of these def files should be included in the program's config file
   but are listed here in case there isn't a config file found or some values 
   are not found in the file.
*/
char def_cfg_filename    [] = "./template_config.txt\0"; // Configuration file name - should be the same as the final pgm name
char def_log_filename    [] = "./logfile.txt\0";         // Default output log file name
char def_det_filename    [] = "./detail_log.txt\0";      // Default detailed log file name
char def_output_filename [] = "./output.txt\0";          // Default program standard processing output file name

char def_data_path       [] = "../data/\0";              // Default path to the input data file location
char def_output_path     [] = "../output/\0";            // Default path to the pgm output file(s)
char def_log_path        [] = "../log/\0";               // Default path to the pgm output log(s)
char def_debug_path      [] = "../log/\0";               // Default path to the pgm's debug listing files

/* Set at runtime, either with the default values (above) or runtime input parms */
char pgm_name[40];                                    // Program name - This program at run time, minus leading non-alpha chars
char cfg_input          [100];                        // Program configuration file name
char log_file           [100];                        // Program log output file
char log_detail         [100];                        // Program detailed log output
char output_file        [100];                        // Program standard output

/* Debugging flags */
int  debug_buf      = 0;                      // Flag: Create a debug output file
int  debug_verbose  = 0;                      // Flag: Produce verbose debug information
int  debug_all      = 0;                      // Flag: Produce all available debug information
int  debug_disp_rec = 0;                      // Flag: Display input record
char input_file_basename[100];                // 1st part of input file name
char input_file_name[100];                    // Input file to this program
char full_input_file_name[100];               // Fully qualified input file name

/*
char todaydate[7];                            // System date
char log_file_name[100];                      // This program's output log file name
char full_log_file[100];                      // Fully qualified output log file name
char debug_file_name[100];                    // This program's debug info file name
char full_debug_file[100];                    // Fully qualified debug info file name

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

int extract_parm(int p_argc,char * p_argv[]);
// int get_parm(p_argc, p_argv)

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


/* ----------------------------------------------------------------------------- */
/* extract_parm: Extract and parse the input parm                                */
/*            (This doesn't include the required file name parm)                 */
/* ----------------------------------------------------------------------------- */
int extract_parm(int p_argc,char * p_argv[])
{
  char *ch;    /* char string pointer for loop */
  int i;       /* loop increment */

  /* Look for the hyphenated parameters and parse them */
  if (strncmp( p_argv[1], "-", 1) == 0 )
  {
    printf(" *** Input parm: <%s>\n", p_argv[1]);

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

      else if ( (strncmp( (p_argv[1] + 3), "D+", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "d+", 2) == 0 )  )
      {
        // Switch ON dispalying input record
        debug_disp_row = 1;       // Produce program debug trace information
        printf(" *** Debug: Switch ON displaying rows before INSERT/UPDATE ****\n");
      }

      // Activate/de-activate displaying dynamic memory release msgs
      else if ( (strncmp( (p_argv[1] + 3), "R-", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "r-", 2) == 0 )  )
      {
        debug_mem = 0;       // Produce program debug trace information
        printf(" *** Debug: Switch OFF displaying dynamic memory release msgs ****\n");
      }
      else if ( (strncmp( (p_argv[1] + 3), "R+", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "r+", 2) == 0 )  )
      {
        debug_mem = 1;       // Produce program debug trace information
        printf(" *** Debug: Switch ON displaying dynamic memory release msgs ****\n");
      }

      // Activate/de-activate verbose mode - displaying detailed logic flow - the small stuff
      else if ( (strncmp( (p_argv[1] + 3), "V-", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "v-", 2) == 0 )  )
      {
        debug_verbose = 0;       // Produce program debug trace information
        printf(" *** Debug: Switch OFF verbose mode - displaying detailed logic flow ****\n");
      }
      else if ( (strncmp( (p_argv[1] + 3), "V+", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "v+", 2) == 0 )  )
      {
        debug_verbose = 1;       // Produce program debug trace information
        printf(" *** Debug: Switch ON verbose mode - displaying detailed logic flow ****\n");
      }

      // THE HAMMER: Activate/de-activate ALL switchable debug statements - including the small stuff
      else if ( (strncmp( (p_argv[1] + 3), "A-", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "a-", 2) == 0 )  )
      {
        debug_all = 0;       // Produce program debug trace information
        printf(" *** Debug: Switch OFF *ALL* switchable debug statements (including the smaller stuff) ****\n");

        debug_buf = 0;       // Produce program debug trace information
        printf(" *** Debug: Switch OFF frequent buffer flushes ****\n");

        debug_disp_row = 0;  // Produce program debug trace information
        printf(" *** Debug: Switch OFF displaying rows before INSERT/UPDATE ****\n");

        debug_mem = 0;       // Produce program debug trace information
        printf(" *** Debug: Switch OFF displaying dynamic memory release msgs ****\n");

        debug_verbose = 0;   // Produce program debug trace information
        printf(" *** Debug: Switch OFF verbose mode - don't display more detailed logic flow ****\n");
      }
      else if ( (strncmp( (p_argv[1] + 3), "A+", 2) == 0 ) || (strncmp( (p_argv[1] + 3), "a+", 2) == 0 )  )
      {
        debug_all = 1;       // Produce program debug trace information
        printf(" *** Debug: Switch ON *ALL* switchable debug statements (including the smaller stuff) ****\n");

        debug_buf = 1;       // Produce program debug trace information
        printf(" *** Debug: Flush buffers frequently ****\n");

        debug_disp_row = 1;       // Produce program debug trace information
        printf(" *** Debug: Switch ON displaying rows before INSERT/UPDATE ****\n");

        debug_mem = 1;       // Produce program debug trace information
        printf(" *** Debug: Switch ON displaying dynamic memory release msgs ****\n");

        debug_verbose = 1;      // Produce program debug trace information
        printf(" *** Debug: Switch ON verbose mode - display more detailed logic flow ****\n");
      }

      else if ( (strncmp( p_argv[1], "-D=n", 4) == 0 ) || (strncmp( p_argv[1], "-d=n", 4) == 0 )  )
      {
        // Specify ?????? debug output
        debug = 1;
        printf(" *** Debug off ****\n");
      }
      */

      else
      {
        printf(" ===== Invalid debug switch type: %s =====\n",p_argv[1]);
        return(1);
      }
    }
    /*
    else if ( (strncmp( p_argv[1], "-db=", 4) == 0 ) || (strncmp( p_argv[1], "-DB=", 4) == 0 )  )
    {
      // Specify the Oracle database to connect to 
      strcpy( db_env, (p_argv[1]+4) );

      if ( (strcmp( db_env, "dev") == 0 ) || (strcmp( db_env, "DEV") == 0 ) )
      {
        userid     = userid_mk10g;
        conn_mikan = conn_mikan_mk10g;
        conn_name  = conn_name_dev;
        //
        //userid=userid_dev;
        //conn_mikan = conn_mikan_dev;
        //
        printf(" **** Connecting to Mikan mk10g - DEV ****\n");
      }
      else if ( (strcmp( db_env, "qa") == 0 ) || (strcmp( db_env, "QA") == 0 ) )
      {
        userid     = userid_qa;
        conn_mikan = conn_mikan_qa;
        conn_name  = conn_name_qa;
        printf(" **** Connecting to Mikan QA. ****\n");
      }
      else if ( (strcmp( db_env, "mig") == 0 ) || (strcmp( db_env, "MIG") == 0 ) )
      {
        userid     = userid_mig;
        conn_mikan = conn_mikan_mig;
        conn_name  = conn_name_mig;
        printf(" **** Connecting to Mikan Migration (mig). ****\n");
      }
      else if ( (strcmp( db_env, "prod") == 0 ) || (strcmp( db_env, "PROD") == 0 )
              )
      {
        userid     = userid_prod;
        conn_mikan = conn_mikan_prd;
        conn_name  = conn_name_prd;
        printf(" **** Connecting to Mikan Prod. ****\n");
      }
      else if ( (strcmp(db_env, "prod_real") == 0) || (strcmp(db_env, "PROD_REAL") == 0) )
      {
        userid     = userid_mikan_prod_real;
        conn_mikan = conn_mikan_prod_real;
        conn_name  = conn_name_prd_real;
        printf(" **** Connecting to Mikan Prod - real userid and tables. ****\n");
      }
      else
        printf(" **** Don't know what I'm connecting to... ****\n");
    }
    else if (  (strncmp( p_argv[1], "-COMMIT", 6 ) == 0 ) || (strncmp( p_argv[1], "-commit", 6 ) == 0 )
            )
    {
      commit_updtes = 1;      // 1 - Commit the database updates 
      printf(" ****** -> Updates flagged for commitment.\n",p_argv[1]);
    }
    else if (  (strncmp( p_argv[1], "-PROD_TABLES", 6 ) == 0 ) || (strncmp( p_argv[1], "-prod_tables", 6 ) == 0 )
            )
    {
      // When specified, use the 'official' production table names instead of the personal/testing table names 
      prod_tables = 1;        // 1 - Use the production/official database tables 
      printf(" ****** -> Using the official/production tables for processing.\n",p_argv[1]);
    }
    */

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
} /*-- get_parm */