  /* timeDate.h
      Personal structure to store and pass derived time and dates in shared formats.
      This is a learning/pratcice exercise in structures and parm passing.
  */
/*
  printf("Localtime, individual elements, all numeric, formatted and adjusted: %d-%02d-%02d %02d:%02d:%02d  ", 
        fn_tml.tm_year + 1900, fn_tml.tm_mon + 1, fn_tml.tm_mday, fn_tml.tm_hour, fn_tml.tm_min, fn_tml.tm_sec);
  printf("- and gmtime (GMT): %d-%02d-%02d %02d:%02d:%02d\n", 
        fn_tmg.tm_year + 1900, fn_tmg.tm_mon + 1, fn_tmg.tm_mday, fn_tmg.tm_hour, fn_tmg.tm_min, fn_tmg.tm_sec);
*/
  
  // construct here and then make it a function parm. 
  typedef struct
  {
    // Default char times & dates
    char LocalTimeStamp[22];  // MariaDB/MySQL text formatted timestamp - local time
    char Localfulltime [21];  // format: dd mmm yyyy hh:mm:ss
    char Localfulltimeh[21];  // format: dd-mmm-yyyy-hh:mm:ss - hyphenated
    char Localdate[12];       // format: dd mmm yyyy
    char Localdateh[12];      // format: dd-mmm-yyyy - hyphenated
    char Localtime[9];        // format: hh:mm:ss
    // char GMTTimeStamp[22];    // MariaDB/MySQL text formatted timestamp - GMT time
    char GMTfulltime[20];     // format: dd mmm yyyy hh:mm:ss
    char GMTfulltimeh[20];    // format: dd-mmm-yyyy-hh:mm:ss
    char ldate5[6];           // format: ddmmm
    char ldate6[7];           // format: ddmm99
    char ldate7[8];           // format: ddmmm99
    char ldate10[10];         // format: dd mmm 99
    // Individual date component values
    int  lyear;               // Local year - int value
    char lyear4[5];           // local 4 figure year - text
    char lyear2[3];           // local 2 figure year - text

    int  lmonth_num;          // Local month number - numeric
    char lmonth2[3];          // local month number - text
    char lmonth_full[12];     // Local full month name (ie. 'November')
    char lmonth_abbv[4];      // Local abbreviated month name (ie. 'Nov')

    int  lday;                // local day - int
    char lday2[3];            // local day - text numeric

    int  l24hour_num;         // local time hour - int
    char l24hour[3];          // local time hour - text
    int  l12hour_num;         // local time hour - int
    char l12hour[3];          // local time hour - text

    int  lmin_num;            // local time min - int
    char lmin[3];             // local time min - text  

    int  lsec_num;            // local time sec - int
    char lsec[3];             // local time sec - text  

    char ltime[9];           // Local time - HH:MM:SS
    char ltime_b[6];          // Local time - HH:MM






    /*
    struct fulldateTime             // Full date
          { int  day;
            char month[12];         //Full month name
            int  yyyy;
            char full[21]           // Full name - ie. '21 January 2025'
          } DateLocalF,             // Local date
            dateGMTF;               // GMT Date (yes,it might be different due to time difference)
    //      custdate;               // Remote OR custom date 
    struct adateTime         // abbreviated date
          {
           char day_f[9];
           char day_ab[4];
           int  day_num;
           
           char month_f[4];            // Abbreviated month name - ie. 'Jan'
           char mnth_ab3[4];
           int  mnth_num;
           
           int  yyyy;
           int  yy_ab;

           int time_hr;
           int time_min;
           int time_sec;

           char commonDate[18];
           char commonTime[10];
           char yymmdd[10];
           char ddmmyy[10];
           char full_time_str[10];

           char commonDateTime[26];
          } Dateloc_ab,             
            DateGMT_ab;
      //    custdate_ab;
    */
  } APP_TIME_DATE;

