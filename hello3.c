/*  hello3.c
    This program, corresponding to chapter 10 in the book, does file 
    manipulation. Added features will be entering files names and 
    processing/debug flags through program input parameters.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Control and processing values (Shamelessly copied from some programs 
   I had produced while still gainfully empolyed so many years ago.               */

/* Buffers and vars that may be referenced throughout this listing                */
/* Some basic input and control buffers, vars, etc. for this program.             */
char todaydate[7];                    // System date
char pgm_name[40];                    // Program name - This program
char input_file_name[100];            // Principle input file to this program
char full_input_file_name[100];       // Fully qualified input file name
char log_file_name[100];              // This program's output log file name
char full_log_file[100];              // Fully qualified output log file name
char debug_file_name[100];            // This program's debug info file name
char full_debug_file[100];            // Fully qualified debug info file name

char input_file_basename[100];        // 1st part of input file name 
char input_file_findtype[100];        // Which FIND file type - from basename
char input_file_findtype_u[100];      // FIND file type - in UPPER CASE

/* Any connection vars needed for a database connection */
//char *userid;
//char *conn_mikan;
//char *conn_name;
//char db_env[26];

// Time buffers to determine local time
struct tm * t_ptr_sys;                       // Time struct to hold the local system time
time_t    sys_t;                             // variable to store the local system time
char now_time_date_loc[26],                  // Store the current local system time &  date
     now_time_date_utc[26];                  // store local time&date in GMT time
     
// More, formatted time vars for display/store/ etc based on the system time sys_t
char rundate[26];
char current_date[12];                       /* Oracle-compatable date  */
char current_date6[7];                       /* Short-form date: yymmdd */
char nowdate[26];
char tmp_mnth[3];

// profiles 
int get_time_date(void);

int main(int argc, char * argv[])
{
  int rc = 0;
  // int param = 0;
  
  /* First, some basic program info, because it's available and I can get it */
  memset(pgm_name,'\0',sizeof(pgm_name));          // Initialize, in case of initiall dirty buffer
  strcpy(pgm_name,argv[0]);                        // Get the input name for this program

  printf("\n Program %s start\n", pgm_name+2);     // Display the program's name 
  printf(" Program Compile Date/Time: %s/%s\n\n",__DATE__, __TIME__); // Display compile time and date


  rc = get_time_date();

  if (rc != 0)
  {
    printf("Couldn't get time(s) and date(s). Return code: %d\n\n\n ", rc);
    return 0;

  }

  /* Now get the specific processing environment(s) vars that this program needs to work in, such as 
     MariaDB, MYSql, etc... database vars, required GUI-based environmental settings, etc. 
     But, don't forget the immediate stuff such as system time and date, file names, etc....
  */


  /*
  // init and set the program run date/time 
  memset(rundate, '\0', sizeof(rundate));
  strcpy(rundate, nowdate);
  // Set up an Oracle-compatable current date 
  memset(current_date, '\0', sizeof(current_date));

  // Set up a 6-char current date yymmdd
  memset(current_date6, '\0', sizeof(current_date6));
  memset(tmp_mnth, '\0', sizeof(tmp_mnth));

  // Set up the Oracle-compatable date
  strncpy(current_date, nowdate+8, 2);
  strcat(current_date, "-");
  strncat(tmp_mnth, nowdate+5, 2);
  if      (strcmp(tmp_mnth, "01") == 0)
    strcat(current_date, "Jan");
  else if (strcmp(tmp_mnth, "02") == 0)
    strcat(current_date, "Feb");
  else if (strcmp(tmp_mnth, "03") == 0)
    strcat(current_date, "Mar");
  else if (strcmp(tmp_mnth, "04") == 0)
    strcat(current_date, "Apr");
  else if (strcmp(tmp_mnth, "05") == 0)
    strcat(current_date, "May");
  else if (strcmp(tmp_mnth, "06") == 0)
    strcat(current_date, "Jun");
  else if (strcmp(tmp_mnth, "07") == 0)
    strcat(current_date, "Jul");
  else if (strcmp(tmp_mnth, "08") == 0)
    strcat(current_date, "Aug");
  else if (strcmp(tmp_mnth, "09") == 0)
    strcat(current_date, "Sep");
  else if (strcmp(tmp_mnth, "10") == 0)
    strcat(current_date, "Oct");
  else if (strcmp(tmp_mnth, "11") == 0)
    strcat(current_date, "Nov");
  else if (strcmp(tmp_mnth, "12") == 0)
    strcat(current_date, "Dec");
  strcat(current_date, "-");
  strncat(current_date, nowdate, 4);

  // Set up the 6-char date - yymmdd 
  memset(tmp_mnth, '\0', sizeof(tmp_mnth));
  strncat(current_date6, nowdate+2, 2);
  strncat(current_date6, nowdate+5, 2);
  strncat(current_date6, nowdate+8, 2);
  strcat( current_date6, "");


  printf("This program is run under the name of: %s\n", argv[0]); 
 
  printf("Program parameters (strings) at run time:\n");
  while (param < argc)
  {
    printf("Parameter %d is %s\n", param, argv[param]);
    param++;
  }
 
*/
 
 
  return 0;
}

int get_time_date(void)
{
  /* get_time_date
     Will derive date and time in all required formats
     There are three methods at the top, for display and experimentation but the method 1 will be used to construct a Date & Time info
     for return to the calling function.

     Note that in most cases the 'time_t' and 'tm' structures are used and 'localtime' is used to derive the current time. Then it's a matter 
      of formatting for the destination.
     
     The derived time in the 'tm' structure needs to be further 'massaged' to accomodate year 2000 and after (as seen in the print statements below)

     */
  //  printf("Year: %d, Month: %d, Day: %d, Hour: %d, Minute:%d, Second: %d, Millisecond: %d",
  //     t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds); // Return year, month, day, hour, minute, second and millisecond in that order

  // DEBUG/DEV:
  // construct here and then make it a function parm. 
  struct timeDateFormats
  {
    //dd-mm-yyyy
    //dd-mmm-yyyy
    //dd-mm
    //struct dd, month[], yyyy
    //struct dd, mmm[4], yyyy
    //struct 
  } timeDate;

  //----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Method 1: 
  time_t fn_time;       // Basic, raw system time in seconds since 1Jan1970. All further times are derived from this
  
  struct tm fn_tml,     // function-specific time - local time
            fn_tmg;     // function-specifioc time - GMT

  time(&fn_time);               // get the basic system time
  fn_tml  = *localtime(&fn_time);     // derive the local time
  fn_tmg  = *gmtime(&fn_time);        // derive the GMT time

  // This method is m ost flexible BUT still needs a bit of massaging to return a proper and accurate date structure to the calling module
  // printf("Local time - raw: %d-%02d-%02d %02d:%02d:%02d\n", fn_tml.tm_year, fn_tml.tm_mon, fn_tml.tm_mday, fn_tml.tm_hour, fn_tml.tm_min, fn_tml.tm_sec);
  printf("Method 1: Localtime, individual elements, all numeric, formatted and adjusted: %d-%02d-%02d %02d:%02d:%02d  ", fn_tml.tm_year + 1900, fn_tml.tm_mon + 1, fn_tml.tm_mday, fn_tml.tm_hour, fn_tml.tm_min, fn_tml.tm_sec);
  printf("- and gmtime (GMT): %d-%02d-%02d %02d:%02d:%02d\n", fn_tmg.tm_year + 1900, fn_tmg.tm_mon + 1, fn_tmg.tm_mday, fn_tmg.tm_hour, fn_tmg.tm_min, fn_tmg.tm_sec);
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------
 
  // ----------------------------------------------------------------------------------
  // Method 2 - this one, however is missing the seconds
  // 
  char  todayDateStr[100];
  char    gmtDateStr[100];
  time_t rawtime;
  struct tm *timeinfo_l,              // time - local time
            *timeinfo_g;              // time - GMT

  time(&rawtime);                     // get system raw time
  timeinfo_l = localtime(&rawtime);                                                           // derive the local time
  strftime(todayDateStr, strlen("DD-MMM-YYYY HH:MM") + 1, "%d-%b-%Y %H:%M", timeinfo_l);      // key step: strftime formats the time elements into todayDatstr

  timeinfo_g = gmtime(&rawtime);                                                              // derive the GMT time
  strftime(gmtDateStr, strlen("DD-MMM-YYYY HH:MM") + 1, "%d-%b-%Y %H:%M", timeinfo_g);        // key step: strftime formats the time elements into todayDatstr


  printf("Method 2: todayDateStr (n.b. no seconds)= %s ", todayDateStr);                      // Print the  date/time string
  printf(" ... gmtDateStr (n.b. no seconds)= %s \n", gmtDateStr);                             // Print the  date/time string
  //-----------------------------------------------------------------------------------
 
  // ----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Method 3: 
  // A quick function applicable to C++ but availabale in C
  // Current time only - no alternate (GMT) available
  // char *pts;        // pointer to the time string
  time_t now;          // current time - computer time
  char * ctime();
  (void) time(&now);
  printf("Method 3: Time now with ctime: %s\n", ctime(&now));
  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------


  /*
  
  memset(now_time_date_loc,'\0',sizeof(now_time_date_loc));          // Initialize, in case of initiall dirty buffer 
  memset(now_time_date_utc,'\0',sizeof(now_time_date_utc));          // Initialize, in case of initiall dirty buffer 
  // Get a system date and time         
  // strcpy(nowdate,u_time_date ());
  sys_t    = time(NULL);                                   // Get the current system time

  // convert to a useful time
  t_ptr_sys = localtime(&sys_t);                           // convert to local time format
  sprintf(now_time_date_loc, "%s", asctime(t_ptr_sys));    // store the current local time
 
  t_ptr_sys = gmtime(&sys_t);                              // convert and store to UCT time format
  sscanf(now_time_date_utc, "%s", asctime(t_ptr_sys));    // store the current UTC time
  printf("system times derived - Local: %s, UTC: %s\n", now_time_date_loc, now_time_date_utc);   // print the derived local time
*/
  
  // display this time retrieved
//   printf("Local time derived: %s\n", asctime(t_ptr_loc));   // default ascii formatted date & time not pretty
//   printf("UTC time derived: %s\n", asctime(t_ptr_utc));   // default ascii formatted date & time not pretty


  
  return 0;
}