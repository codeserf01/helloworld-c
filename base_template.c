/*  hello3.c
    This program, corresponding to chapter 10 in the book, does file
    manipulation. Added features will be entering files names and
    processing/debug flags through program input parameters.
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

char pgm_name[40];                            // Program name - This program
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

int main(int argc, char *argv[])
{
  int i = 0;                                               // Generic increment counter

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
  while ( isalnum(argv[0][i] ) == 0)                       // Detect non-alphanumeric char, skip of found
    i++;
  strcpy(pgm_name, argv[0]+i);                             // Get this program's name at runtime
    
  printf("\n Program: %s\n", pgm_name);                    // Display this program's name but skip the './' leading characters
  printf(" Program Compile Date/Time: %s/%s\n\n", __DATE__, __TIME__);  // Display this program's compile date and time

  printf(" Current time: (local): %s, ", loc_Time_str);    // Print out the 'local time' string
  printf(" (UTC): %s", UTC_Time_str);                      // include UTC time to the string 
  printf("\n\n");                                          // finish line with line feed(s) 





  


  /* Now get the specific processing environment(s) vars that this program needs to work in,
     such as
     MariaDB, MYSql, etc... database vars, required GUI-based environmental settings, etc.
     But, don't forget the immediate stuff such as system time and date, file names, etc....
  */

  return 0;
}


/*
int get_weekday(char * str) {
  struct tm tm;
  memset((void *) &tm, 0, sizeof(tm));
  if (strptime(str, "%d-%m-%Y", &tm) != NULL) 
  {
    time_t t = mktime(&tm);
    if (t >= 0) {
      return localtime(&t)->tm_wday; // Sunday=0, Monday=1, etc.
    }
  }
  return -1;
}
*/
/*
const char *wd(int year, int month, int day) {
  // using C99 compound literals in a single line: notice the splicing 
  return ((const char *[])                                         \
          {"Monday", "Tuesday", "Wednesday",                       \
           "Thursday", "Friday", "Saturday", "Sunday"})[           \
      (                                                            \
          day                                                      \
        + ((153 * (month + 12 * ((14 - month) / 12) - 3) + 2) / 5) \
        + (365 * (year + 4800 - ((14 - month) / 12)))              \
        + ((year + 4800 - ((14 - month) / 12)) / 4)                \
        - ((year + 4800 - ((14 - month) / 12)) / 100)              \
        + ((year + 4800 - ((14 - month) / 12)) / 400)              \
        - 32045                                                    \
      ) % 7];
}
*/

/*
int inittimestruct(&runtimeDate )
{
  // comment out the call to the current time function for now

  rc = get_time_date(&runtimeDate);
  if (rc != 0)
  {
    printf("Couldn't get time(s) and date(s). Return code: %d\n\n\n ", rc);
    return 0;
  }
  else
  {
    printf("Available date/time strings:\n");
    printf("LocalTimeStamp (yyyymmddhhmmss): >%s<\n",      runtimeDate.LocalTimeStamp);
    printf("Localfulltime (dd mmm yyyy hh:mm:ss): >%s<\n", runtimeDate.Localfulltime);
    printf("Localfulltimeh(dd-mmm-yyyy-hh:mm:ss): >%s<\n", runtimeDate.Localfulltimeh);
    printf("Localfulltimes(dd/mmm/yyyy/hh:mm:ss): >%s<\n", runtimeDate.Localfulltimes);
    printf("yearmnthday   (yyyy mmm dd hh:mm:ss): >%s<\n", runtimeDate.lyearmnthdaytm);
    printf("yearmnthdayh  (yyyy-mmm-dd hh:mm:ss): >%s<\n", runtimeDate.lyearmnthdaytmh);
    printf("yearmnthdays  (yyyy/mmm/dd hh:mm:ss): >%s<\n", runtimeDate.lyearmnthdaytms);
    printf("yrmndys       (yymmdd)              : >%s<\n", runtimeDate.lyrmndys);
    printf("Localdate     (dd mmm yyyy)         : >%s<\n", runtimeDate.Localdate);
    printf("Localdateh    (dd-mmm-yyyy)         : >%s<\n", runtimeDate.Localdateh);
    printf("Localdates    (dd/mmm/yyyy)         : >%s<\n", runtimeDate.Localdates);
    printf("Localtime     (hh:mm:ss)            : >%s<\n", runtimeDate.Localtime);
    // printf("GMTTimeStamp  (xx xxx xxx hh:mm:ss): >%s<\n",  runtimeDate.GMTTimeStamp);
    printf("GMTfulltime   (dd mmm yyyy hh:mm:ss): >%s<\n", runtimeDate.GMTfulltime);
    printf("GMTfulltimeh  (dd-mmm-yyyy-hh:mm:ss): >%s<\n", runtimeDate.GMTfulltimeh);
    printf("ldate5         (ddmmm)              : >%s<\n", runtimeDate.ldate5);
    printf("ldate6         (ddmmyy)             : >%s<\n", runtimeDate.ldate6);
    printf("ldate7         (ddmmm99)            : >%s<\n", runtimeDate.ldate7);
    printf("ldate10        (dd mmm yy)          : >%s<\n", runtimeDate.ldate10);
    printf("lyear            (int): >%d<, lyear4 (yyyy): >%s< lyear2 (yy): >%s<\n", runtimeDate.lyear, runtimeDate.lyear4, runtimeDate.lyear2);
    printf("lmonth_num       (int): >%d<, lmonth2(mmm) : >%s<\n", runtimeDate.lmonth_num, runtimeDate.lmonth2);
    printf("month_full (text name): >%s<, month_abbv (mmm): >%s<\n", runtimeDate.lmonth_full, runtimeDate.lmonth_abbv);
    printf("lday             (int): >%d<, lday2  (dd): >%s<\n", runtimeDate.lday, runtimeDate.lday2);
    printf("Hour: l24hour_num (int):>%d<, (text):>%s<,\n      l12hour_num (int):>%d<,  (text):>%s<\n", 
                                                     runtimeDate.l24hour_num, runtimeDate.l24hour, runtimeDate.l12hour_num, runtimeDate.l12hour);
    printf("lmin_num    (int):>%d<, (text):>%s<,\n", runtimeDate.lmin_num,  runtimeDate.lmin);
    printf("lsec_num    (int):>%d<, (text):>%s<\n",  runtimeDate.lsec_num,  runtimeDate.lsec);
  }
  
  return 0;
}
*/
