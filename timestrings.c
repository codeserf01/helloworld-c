/*  timestrings.c
    This program, corresponding to chapter 10 in the book, does file
    manipulation. Added features will be entering files names and
    processing/debug flags through program input parameters.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
// #include "timeDate.h"

char pgm_name[40];              // Program name - This program

// Time buffers to determine local time
// struct tm *t_ptr_sys;       // Time struct to hold the local system time
// time_t sys_t;               // variable to store the local system time
// char now_time_date_loc[26], // Store the current local system time &  date
//    now_time_date_utc[26];  // store local time&date in GMT time

int main(int argc, char *argv[])
{
  /* First, some basic program info, because it's available and I can get it */
  memset(pgm_name, '\0', sizeof(pgm_name));                             // Initialize, in case of initiall dirty buffer
  strcpy(pgm_name, argv[0]);                                            // Get this program's name at runtime
  printf("\n Program %s start\n", pgm_name + 2);                        // Display this program's name
  printf(" Program Compile Date/Time: %s/%s\n\n", __DATE__, __TIME__);  // Display this program's compile date and time
 
  /*
  *  time_t()) will give you the system's 'raw' time at the call's time.
  *  Then use localtime()/gmtime(), etc. to produce local, GMT, etc times, using the time()'s returned value.
  * 
  *  For future consideration:
  *  If you're using pointers only to refer to time, remember that localtime() or gmtime() both return a pointer to the SAME 
  *  system buffer!! Make a COPY of this time/date data for later use if need. This buffer will change with any subsequent 
  *  system calls (ie. localtime(), gmtime(), etc...
  *  So, if you want to use local time, be sure that the last call was localtime(). Conversely, if you later want UTC/GMT time,
  *  be sure that you explicitly call gmtime() immediately before. Better yet, make a copy when you make a call.
  *  Remember that both need the time() call to provide the initial system time, but that need not change (so you can work off the 
  *  same initial system time call as often as you want.
  */

  time_t   system_Time = time(NULL);                    // define and init a local copy of the system raw time 
  struct  tm loc_Time  = *localtime(&system_Time);      // define and init a local copy of the derived local time
  struct  tm UTC_Time  = *gmtime(&system_Time);         // define and init a local copy of the derived GMT/UTC time structure
  char loc_Time_str[25];                                // local date & time string (used later)
  char UTC_Time_str[25];                                // GMT/UTC date & time string (used later)
  char gen_datetimestr[50];                             // generic date/time string
  
  /*--------------------------------------------------------------------------------------------------------------------*/
  // 'Old School' alternative to above: Define first (usually as a Global) and then assign values:
  // system_Time = time(NULL);                             // Initialize the local time to the system time
  // struct  tm loc_Time;                                  // define a local copy of the derived local time
  // struct  tm UTC_Time;                                  // define a local copy of the derived GMT/UTC time structure
  // loc_Time   = *localtime(&system_Time);                // initialize a copy of the derived local time structure
  // UTC_Time   = *gmtime(&system_Time);                   // initialize a copy of the derived GMT time structure
  /*--------------------------------------------------------------------------------------------------------------------*/

  memset(loc_Time_str, '\0', sizeof(loc_Time_str));        // initialize first 
  memset(UTC_Time_str, '\0', sizeof(UTC_Time_str));        // initialize first
  memset(gen_datetimestr, '\0', sizeof(gen_datetimestr));  // initialize first

  strncpy(loc_Time_str, asctime(&loc_Time), 24);        // create a basic local time date/time string, leaving out the trailing '\n' char
  strncpy(UTC_Time_str, asctime(&UTC_Time), 24);        // create a basic GMT date/time string, leaving out the trailing '\n' char
  // sprintf(UTC_Time_str, "%s", asctime(&UTC_Time));

  printf("Current time: (local): %s, ", loc_Time_str);    // Print out the 'local time' string
  printf("(UTC): %s\n\n", UTC_Time_str);                    // add to the line the UTC time string

  
  printf("Some date & time examples using the strftime() date/time formatting function:\n");
  strftime(gen_datetimestr, sizeof(gen_datetimestr), "%c", &loc_Time);
  printf("Date & time using strftime - c option - Local Date and time: %s \n", gen_datetimestr);

  memset(gen_datetimestr, '\0', sizeof(gen_datetimestr));     // initialize first
  strftime(gen_datetimestr, sizeof(gen_datetimestr), "%F", &loc_Time);
  printf("Date & time using strftime - F option (YYYY-MM-DD): %s \n", gen_datetimestr);

  // Formatted date and time: %a -Weekday(full), %d - day of the month, %B -Month(abbreviation), %Y -year(YYYY), %T - time
  memset(gen_datetimestr, '\0', sizeof(gen_datetimestr));     // initialize first
  strftime(gen_datetimestr, sizeof(gen_datetimestr), "%A, %d %B %Y - %T", &loc_Time);
  printf("Date & time using strftime - A, d, B, Y, T options: %s \n", gen_datetimestr);
  
  memset(gen_datetimestr, '\0', sizeof(gen_datetimestr));     // initialize first
  strftime(gen_datetimestr, sizeof(gen_datetimestr), "%A, %B %d %Y - %T", &loc_Time);
  printf("Date & time using strftime - A, B, d, Y, T options: %s \n", gen_datetimestr);


  memset(gen_datetimestr, '\0', sizeof(gen_datetimestr));     // initialize first
  strftime(gen_datetimestr, sizeof(gen_datetimestr), "%A, %B %d %Y - %r", &loc_Time);
  printf("Date & time using strftime - A, B, d, Y, r options: %s \n", gen_datetimestr);


  // use of ctime()
  char * timestr;
  timestr = ctime(&system_Time);
  printf("\n\n>>>>>>> Current ctime is %s<<<<<<<<\n", timestr);




//  printf(">>>>>>> Current asctime is %s<<<<<<<<\n", asctime(&sys_t));
//  printf(">>>>>>> Current gmtime is %s<<<<<<<<\n", gmtime(&sys_t));
//  printf(">>>>>>> Current localtime is %s<<<<<<<<\n", localtime(&sys_t));


  


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
