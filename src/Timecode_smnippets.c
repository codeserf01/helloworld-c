
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
/* timecode snippets:
    There are three methods at the top, for display and experimentation but the method 1 will be used to construct a Date & Time info
     for return to the calling function.

     Note that in most cases the 'time_t' and 'tm' structures are used and 'localtime' is used to derive the current time. Then it's a matter 
      of formatting for the destination.
     
     The derived time in the 'tm' structure needs to be further 'massaged' to accomodate year 2000 and after (as seen in the print statements below)
*/



//----------------------------------------------------------------------------------------------------------------------------------------------------------------
  // Method 1: 
  time_t fn_time;       // Basic, raw system time in seconds since 1Jan1970. All further times are derived from this
  
  struct tm fn_tml,     // function-specific time - local time
            fn_tmg;     // function-specifioc time - GMT

  time(&fn_time);               // get the basic system time
  fn_tml  = *localtime(&fn_time);     // derive the local time
  fn_tmg  = *gmtime(&fn_time);        // derive the GMT time

  // This method is most flexible BUT still needs a bit of massaging to return a proper and accurate date structure to the calling module
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
