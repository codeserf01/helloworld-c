
#include <time.h>
#include "timeDate.h"

int get_time_date(APP_TIME_DATE *fn_timeDate)
{
  /* get_time_date
     Will derive date and time in all required formats
     */
  //  printf("Year: %d, Month: %d, Day: %d, Hour: %d, Minute:%d, Second: %d, Millisecond: %d",
  //     t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds); // Return year, month, day, hour, minute, second and millisecond in that order
  //----------------------------------------------------------------------------------------------------------------------------------------------------------------

  time_t fn_time;          // Basic, raw system time in seconds since 1Jan1970. All further times are derived from this.

  struct tm fn_tml,       // function-specific time - local time
            fn_tmg;       // function-specifioc time - GMT

  char l_yearstr[23];
  char l_monthstr[23];
  char l_daystr[23];
  char l_12hrstr[23];
  char l_24hrstr[23];
  char l_minstr[23];
  char l_secstr[23];
  int  l_mnth;

  char g_yearstr[23];
  char g_monthstr[23];
  char g_daystr[23];
  char g_hrstr[23];
  char g_minstr[23];
  char g_secstr[23];
  int  g_mnth;

  // Get a system date and time
  time(&fn_time);                // get the basic system time
  fn_tml = *localtime(&fn_time); // derive the local time
  fn_tmg = *gmtime(&fn_time);    // derive the GMT time

  // This method is most flexible BUT still needs a bit of massaging to return a proper and accurate date structure to the calling module
  // printf("Local time - raw: %d-%02d-%02d %02d:%02d:%02d\n", fn_tml.tm_year, fn_tml.tm_mon, fn_tml.tm_mday, fn_tml.tm_hour, fn_tml.tm_min, fn_tml.tm_sec);
  //printf("Localtime, individual elements, all numeric, formatted and adjusted: %d-%02d-%02d %02d:%02d:%02d  ",
  //       fn_tml.tm_year + 1900, fn_tml.tm_mon + 1, fn_tml.tm_mday, fn_tml.tm_hour, fn_tml.tm_min, fn_tml.tm_sec);
  //printf("- and gmtime (GMT): %d-%02d-%02d %02d:%02d:%02d\n\n",
  //       fn_tmg.tm_year + 1900, fn_tmg.tm_mon + 1, fn_tmg.tm_mday, fn_tmg.tm_hour, fn_tmg.tm_min, fn_tmg.tm_sec);

  // alternative display of time retrieved
  //   printf("Local time derived: %s\n", asctime(t_ptr_loc));   // default ascii formatted date & time not pretty
  //   printf("UTC time derived: %s\n", asctime(t_ptr_utc));   // default ascii formatted date & time not pretty

  //-----------------------------------------------------------------------------------------------------------------------------------------------------------------

  // fn_timeDate:
  // Initialize needed parts of fn_timeDate time/date structure
  memset(fn_timeDate->LocalTimeStamp, '\0', sizeof(fn_timeDate->LocalTimeStamp));  // Maria/MySQL timestamp format: yyyymmddhhmmss

  memset(fn_timeDate->Localfulltime,  '\0', sizeof(fn_timeDate->Localfulltime));   // Local full date & time: dd mmm yyyy hh:mm:ss
  memset(fn_timeDate->Localfulltimeh, '\0', sizeof(fn_timeDate->Localfulltimeh));  // Local full date & time: dd-mmm-yyyy-hh:mm:ss
  memset(fn_timeDate->lyearmnthdaytm, '\0', sizeof(fn_timeDate->lyearmnthdaytm));  // Local year mnth day hh:mm:ss
  memset(fn_timeDate->lyearmnthdaytmh,'\0', sizeof(fn_timeDate->lyearmnthdaytmh)); // Local year mnth day dd-mmm-yyyy hh:mm:ss - hyphenated
  memset(fn_timeDate->lyearmnthdaytms,'\0', sizeof(fn_timeDate->lyearmnthdaytms)); // Local year mnth day dd/mmm/yyyy hh:mm:ss - hyphenated

  memset(fn_timeDate->lyrmndys,       '\0', sizeof(fn_timeDate->lyrmndys));        // Local year mnth day yymmdd
  memset(fn_timeDate->Localdate,      '\0', sizeof(fn_timeDate->Localdate));       // Local date:             dd mmm yyyy
  memset(fn_timeDate->Localdateh,     '\0', sizeof(fn_timeDate->Localdateh));      // Local date:             dd-mmm-yyyy 
  memset(fn_timeDate->Localdates,     '\0', sizeof(fn_timeDate->Localdates));      // Local date:             dd-mmm-yyyy 
  memset(fn_timeDate->Localtime,      '\0', sizeof(fn_timeDate->Localtime));       // Local time:             hh:mm:ss
 
  // char GMTTimeStamp[22];    // MariaDB/MySQL text formatted timestamp - GMT time
  memset(fn_timeDate->GMTfulltime,    '\0', sizeof(fn_timeDate->GMTfulltime));    // GMT date & time:        dd mmm yyyy hh:mm:ss
  memset(fn_timeDate->GMTfulltimeh,   '\0', sizeof(fn_timeDate->GMTfulltimeh));   // GMT date & time:        dd mmm yyyy hh:mm:ss
  memset(fn_timeDate->ldate5,         '\0', sizeof(fn_timeDate->ldate5));         // Format: date5:          ddmmm
  memset(fn_timeDate->ldate6,         '\0', sizeof(fn_timeDate->ldate6));         // Default date6:          ddmmyy
  memset(fn_timeDate->ldate7,         '\0', sizeof(fn_timeDate->ldate7));         // Default date7:          ddmmm99
  memset(fn_timeDate->ldate10,        '\0', sizeof(fn_timeDate->ldate10));        // Default date10:         dd mmm yy
 
 
  fn_timeDate->lyear = 0;                                                         // Local year - int
  memset(fn_timeDate->lyear4,         '\0', sizeof(fn_timeDate->lyear4));         // Local year - text       yyyy
  memset(fn_timeDate->lyear2,         '\0', sizeof(fn_timeDate->lyear2));         // local year - text       yy
  fn_timeDate->lmonth_num = 0;                                                    // Local month - int
  memset(fn_timeDate->lmonth2,        '\0', sizeof(fn_timeDate->lmonth2));        // Local month num - text
  memset(fn_timeDate->lmonth_full,    '\0', sizeof(fn_timeDate->lmonth_full));    // Default - Full month name        (ie. 'November')
  memset(fn_timeDate->lmonth_abbv,    '\0', sizeof(fn_timeDate->lmonth_abbv));    // Default - Abbreviated month name (ie. 'Nov')
  fn_timeDate->lday = 0;                                                          // Local day of month - int
  memset(fn_timeDate->lday2,          '\0', sizeof(fn_timeDate->lday2));          // Local day of month - text

  memset(fn_timeDate->ltime,          '\0', sizeof(fn_timeDate->ltime));          // Local time - text  HH:MM:SS
  memset(fn_timeDate->ltime_b,        '\0', sizeof(fn_timeDate->ltime_b));        // Local time - text HH:MM

  fn_timeDate->l24hour_num = 0;                                                     // Local time hour   - int
  memset(fn_timeDate->l24hour,        '\0', sizeof(fn_timeDate->l24hour));          // Local time hour   - text HH
  memset(fn_timeDate->l12hour,        '\0', sizeof(fn_timeDate->l12hour));          // Local time hour   - text HH
  fn_timeDate->lmin_num  = 0;                                                     // Local time minute - int
  memset(fn_timeDate->lmin,           '\0', sizeof(fn_timeDate->lmin));           // Local time minute - text MM
  fn_timeDate->lsec_num  = 0;                                                     // Local time second - int
  memset(fn_timeDate->lsec,           '\0', sizeof(fn_timeDate->lsec));           // Local time second - text MM



  // Local - in-pgm: Initialize temp strings for date & time
  memset(l_yearstr,  '\0', sizeof(l_yearstr));      // local time
  memset(l_monthstr, '\0', sizeof(l_monthstr));
  memset(l_daystr,   '\0', sizeof(l_daystr));
  memset(l_12hrstr,  '\0', sizeof(l_12hrstr));
  memset(l_24hrstr,  '\0', sizeof(l_24hrstr));
  memset(l_minstr,   '\0', sizeof(l_minstr));
  memset(l_secstr,   '\0', sizeof(l_secstr));
  l_mnth = 0;

  memset(g_yearstr,  '\0', sizeof(g_yearstr));      // GMT time
  memset(g_monthstr, '\0', sizeof(g_monthstr));
  memset(g_daystr,   '\0', sizeof(g_daystr));
  memset(g_hrstr,    '\0', sizeof(g_hrstr));
  memset(g_minstr,   '\0', sizeof(g_minstr));
  memset(g_secstr,   '\0', sizeof(g_secstr));
  g_mnth = 0;

  /* convert date & time to strings; convert in order of year, month, day, hr, min, sec
  */
  // year: set up the strings for later use, making adjustments
  sprintf(l_yearstr, "%d ", fn_tmg.tm_year + 1900);
  fn_timeDate->lyear = fn_tmg.tm_year + 1900;
  strncat(fn_timeDate->lyear4, l_yearstr,   4);
  strncat(fn_timeDate->lyear2, l_yearstr+2, 2);

  // month:
  fn_timeDate->lmonth_num = fn_tmg.tm_mon + 1;
  // strncat(fn_timeDate->lmonth3, l_monthyearstr,   4);

  // sprintf(monthstr,"%d", fn_tmg.tm_mon + 1);
  // strcat(fn_timeDate->Localfulltime," ");           // Initialize default format Local full date & time: dd mmm yyyy hh:mm:ss

  // Local - Convert numeric month to name. Remember months are numbered by the system starting with 0
  l_mnth = fn_tml.tm_mon;
  switch (l_mnth)
  {
  case 0:
  {
    strcat(l_monthstr, "Jan");
    strcat(fn_timeDate->lmonth2, "01");
    strcat(fn_timeDate->lmonth_full, "January");
    strcat(fn_timeDate->lmonth_abbv, "Jan");
    // strcat();
    break;
  }
  case 1:
  {
    strcat(l_monthstr, "Feb");
    strcat(fn_timeDate->lmonth2, "02");
    strcat(fn_timeDate->lmonth_full, "February");
    strcat(fn_timeDate->lmonth_abbv, "Feb");
    break;
  }
  case 2:
  {
    strcat(l_monthstr, "Mar");
    strcat(fn_timeDate->lmonth2, "03");
    strcat(fn_timeDate->lmonth_full, "March");
    strcat(fn_timeDate->lmonth_abbv, "Mar");
    break;
  }
  case 3:
  {
    strcat(l_monthstr, "Apr");
    strcat(fn_timeDate->lmonth2, "04");
    strcat(fn_timeDate->lmonth_full, "April");
    strcat(fn_timeDate->lmonth_abbv, "Apr");
    break;
  }
  case 4:
  {
    strcat(l_monthstr, "May");
    strcat(fn_timeDate->lmonth2, "05");
    strcat(fn_timeDate->lmonth_full, "May");
    strcat(fn_timeDate->lmonth_abbv, "May");
    break;
  }
  case 5:
  {
    strcat(l_monthstr, "Jun");
    strcat(fn_timeDate->lmonth2, "06");
    strcat(fn_timeDate->lmonth_full, "June");
    strcat(fn_timeDate->lmonth_abbv, "Jun");
    break;
  }
  case 6:
  {
    strcat(l_monthstr, "Jul");
    strcat(fn_timeDate->lmonth2, "07");
    strcat(fn_timeDate->lmonth_full, "July");
    strcat(fn_timeDate->lmonth_abbv, "Jul");
    break;
  }
  case 7:
  {
    strcat(l_monthstr, "Aug");
    strcat(fn_timeDate->lmonth2, "08");
    strcat(fn_timeDate->lmonth_full, "August");
    strcat(fn_timeDate->lmonth_abbv, "Aug");
    break;
  }
  case 8:
  {
    strcat(l_monthstr, "Sep");
    strcat(fn_timeDate->lmonth2, "09");
    strcat(fn_timeDate->lmonth_full, "September");
    strcat(fn_timeDate->lmonth_abbv, "Sep");
    break;
  }
  case 9:
  {
    strcat(l_monthstr, "Oct");
    strcat(fn_timeDate->lmonth2, "10");
    strcat(fn_timeDate->lmonth_full, "October");
    strcat(fn_timeDate->lmonth_abbv, "Oct");
    break;
  }
  case 10:
  {
    strcat(l_monthstr, "Nov");
    strcat(fn_timeDate->lmonth2, "11");
    strcat(fn_timeDate->lmonth_full, "November");
    strcat(fn_timeDate->lmonth_abbv, "Nov");
    break;
  }
  case 11:
  {
    strcat(l_monthstr, "Dec");
    strcat(fn_timeDate->lmonth2, "12");
    strcat(fn_timeDate->lmonth_full, "December");
    strcat(fn_timeDate->lmonth_abbv, "Dec");
    break;
  }
  default:
  {
    return 20;
    break;
  }
  }

  // Day:
  // Accomodate for 1 and 2 character days
  if (fn_tmg.tm_mday < 10)
  {
    sprintf(l_daystr, "%02d", fn_tmg.tm_mday);     // convert day integer to string - double digit day
  }
    else
  {
    sprintf(l_daystr, "%d", fn_tmg.tm_mday);      // convert day integer to string - single digit day
  }
  fn_timeDate->lday = fn_tmg.tm_mday;             // Local day of the month - int
  strncat(fn_timeDate->lday2, l_daystr, 2);       // Local day of the month - text version

  // time:
  fn_timeDate->l24hour_num = fn_tml.tm_hour;
  sprintf(l_24hrstr, "%d", fn_tml.tm_hour);                 // convert local time hour to string - single digit day
  strncpy(fn_timeDate->l24hour, l_24hrstr, 2 );
  







  // derive the 12hr clock equivalent and save it
  if (fn_tml.tm_hour > 12)
  {
    fn_timeDate->l12hour_num = fn_tml.tm_hour - 12;
    sprintf(l_12hrstr, "%02d", fn_tml.tm_hour - 12);
    strncpy(fn_timeDate->l12hour, l_12hrstr, 2 );
  }
  else
  {
    fn_timeDate->l12hour_num = fn_tml.tm_hour;
    sprintf(l_24hrstr, "%02d", fn_tml.tm_hour);                 // convert local time hour to string - single digit day
  }

  fn_timeDate->lmin_num = fn_tml.tm_min;
  sprintf(l_minstr, "%02d", fn_tml.tm_min);                // convert local time minute to string - single digit day
  strncpy(fn_timeDate->lmin, l_minstr, 2 );
  
  fn_timeDate->lsec_num = fn_tml.tm_sec;
  if (fn_timeDate->lsec_num != 0)
  {
    sprintf(l_secstr, "%02d", fn_tml.tm_sec);                 // convert local time second to string - single digit day
    strncpy(fn_timeDate->lsec, l_secstr, 2 );
  }
  else
  {
    strcpy(l_secstr, "00");
    strncpy(fn_timeDate->lsec, l_secstr, 2 );
  }

  int wkdaynum;
  printf("Weekday number: %s", "2" );





  strncpy(fn_timeDate->ltime, "%d", fn_tmg.tm_mday);      // convert day integer to string - single digit day

  // Build the output local time/date
  // Full time - no hyphens
  strcpy(fn_timeDate->Localfulltime, fn_timeDate->lday2);
  strcat(fn_timeDate->Localfulltime, " ");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, fn_timeDate->lmonth_abbv);
  strcat(fn_timeDate->Localfulltime, " ");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, fn_timeDate->lyear4);
  strcat(fn_timeDate->Localfulltime, " ");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, fn_timeDate->l24hour);
  strcat(fn_timeDate->Localfulltime, ":");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, fn_timeDate->lmin);
  strcat(fn_timeDate->Localfulltime, ":");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, fn_timeDate->lsec);

  // hyphenated date/time
  strcpy(fn_timeDate->Localfulltimeh, fn_timeDate->lday2);
  strcat(fn_timeDate->Localfulltimeh, "-");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimeh, fn_timeDate->lmonth_abbv);
  strcat(fn_timeDate->Localfulltimeh, "-");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimeh, fn_timeDate->lyear4);
  strcat(fn_timeDate->Localfulltimeh, "-");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimeh, fn_timeDate->l24hour);
  strcat(fn_timeDate->Localfulltimeh, ":");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimeh, fn_timeDate->lmin);
  strcat(fn_timeDate->Localfulltimeh, ":");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimeh, fn_timeDate->lsec);

  // slash/stroke delimited date/time
  strcpy(fn_timeDate->Localfulltimes, fn_timeDate->lday2);
  strcat(fn_timeDate->Localfulltimes, "/");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimes, fn_timeDate->lmonth_abbv);
  strcat(fn_timeDate->Localfulltimes, "/");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimes, fn_timeDate->lyear4);
  strcat(fn_timeDate->Localfulltimes, " ");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimes, fn_timeDate->l24hour);
  strcat(fn_timeDate->Localfulltimes, ":");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimes, fn_timeDate->lmin);
  strcat(fn_timeDate->Localfulltimes, ":");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltimes, fn_timeDate->lsec);

  // year month day time - no hyphens
  strcpy(fn_timeDate->lyearmnthdaytm, fn_timeDate->lyear4);
  strcat(fn_timeDate->lyearmnthdaytm, " ");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytm, fn_timeDate->lmonth_abbv);
  strcat(fn_timeDate->lyearmnthdaytm, " ");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytm, fn_timeDate->lday2);
  strcat(fn_timeDate->lyearmnthdaytm, " ");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytm, fn_timeDate->l24hour);
  strcat(fn_timeDate->lyearmnthdaytm, ":");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytm, fn_timeDate->lmin);
  strcat(fn_timeDate->lyearmnthdaytm, ":");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytm, fn_timeDate->lsec);

  // year month day time - hyphenated
  strcpy(fn_timeDate->lyearmnthdaytmh, fn_timeDate->lyear4);
  strcat(fn_timeDate->lyearmnthdaytmh, "-");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytmh, fn_timeDate->lmonth_abbv);
  strcat(fn_timeDate->lyearmnthdaytmh, "-");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytmh, fn_timeDate->lday2);
  strcat(fn_timeDate->lyearmnthdaytmh, " ");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytmh, fn_timeDate->l24hour);
  strcat(fn_timeDate->lyearmnthdaytmh, ":");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytmh, fn_timeDate->lmin);
  strcat(fn_timeDate->lyearmnthdaytmh, ":");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytmh, fn_timeDate->lsec);

  // year month day time - slashes
  strcpy(fn_timeDate->lyearmnthdaytms, fn_timeDate->lyear4);
  strcat(fn_timeDate->lyearmnthdaytms, "/");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytms, fn_timeDate->lmonth_abbv);
  strcat(fn_timeDate->lyearmnthdaytms, "/");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytms, fn_timeDate->lday2);
  strcat(fn_timeDate->lyearmnthdaytms, " ");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytms, fn_timeDate->l24hour);
  strcat(fn_timeDate->lyearmnthdaytms, ":");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytms, fn_timeDate->lmin);
  strcat(fn_timeDate->lyearmnthdaytms, ":");        // Formatting: insert space
  strcat(fn_timeDate->lyearmnthdaytms, fn_timeDate->lsec);

  // year month day time - slashes
  strcpy(fn_timeDate->lyrmndys, fn_timeDate->lyear2); // yymmdd
  strcat(fn_timeDate->lyrmndys, fn_timeDate->lmonth2);
  strcat(fn_timeDate->lyrmndys, fn_timeDate->lday2);

/*
  strcat(fn_timeDate->Localfulltime, l_daystr);   // build the full day/time, starting with the day
  strcat(fn_timeDate->Localfulltime, " ");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, l_monthstr); // Insert the month string
  strcat(fn_timeDate->Localfulltime, " ");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, l_yearstr);    // Insert year string
  // strcat(fn_timeDate->Localfulltime, " ");      // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, l_24hrstr);    // Insert year string
  strcat(fn_timeDate->Localfulltime, ":");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, l_minstr);   // Insert year string
  strcat(fn_timeDate->Localfulltime, ":");        // Formatting: insert space
  strcat(fn_timeDate->Localfulltime, l_secstr);   // Insert year string
*/



  /*

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
