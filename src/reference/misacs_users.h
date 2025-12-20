/* MISACS_USER */
struct
{
  /* ID,party no,MISACS_init,MISACSname,First Name,Family Name,MISACS_org,CIM Id,Comments */
  int     lineid;              /*  NOT Null */
  int     party_num;           /*  nullable */
  char    misacs_id[11];       /*  nullable */
  char    misacs_org[21];      /*  nullable */
  char    cim_id[15];          /*  nullable */
  VARCHAR full_name[101];      /* varchar(100) nullable */
  VARCHAR first_name[101];     /* varchar(100) nullable */
  VARCHAR last_name[101];      /* varchar(100) nullable */
  VARCHAR user_comment[301];   /* varchar(300) nullable */
} misacs_user;

/* NULL indicators */
struct
{
  short party_num;
  short misacs_id;
  short misacs_org;
  short cim_id;
  short full_name;
  short first_name;
  short last_name;
  short user_comment;
} misacs_user_ind;
