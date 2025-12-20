/* ---------------------------------------------
 * CMS3 - tables and sequencers
 * --------------------------------------------- */

  /* mtrl */
  struct 
  {
    int     mtrl_nbr;			        		/* number(11,0)		not null enable		*/
    VARCHAR mtrl_idx_list_txt[1001];			/* varchar2(1000 byte)	 				*/
    VARCHAR cntnr_nbr_rng_txt[1001];			/* varchar2(1000 byte)					*/
    VARCHAR cntnr_brcde_nbr_rng_txt[1001];		/* varchar2(1000 byte)					*/
    VARCHAR mtrl_nte_txt[2001]; 				/* clob									*/
    int     wrk_ordr_nbr;	 					/* number(11,0)		not null enable		*/
    int     acs_rtctn_cde;						/* number(6,0)							*/
  } mtrl;

  /* mtrl indicators */
  struct 
  {
    short   mtrl_idx_list_txt;
    short   cntnr_nbr_rng_txt;
    short   cntnr_brcde_nbr_rng_txt;
    short   mtrl_nte_txt;
    short   acs_rtctn_cde;
  } mtrl_ind;


  /* prjct */
  struct 
  {
    int     prjct_nbr;							/* number(11,0)		   not null enable  */
    VARCHAR prjct_nme_txt[129];					/* varchar2(128 byte)  not null enable	*/
    VARCHAR prjct_nte_txt[2001];				/* clob                                 */ 
    char    prjct_crte_dte[26];					/* date			       not null enable  */
    int     prjct_crte_by_party_nbr;			/* number(11,0)	       not null enable  */
    char    prjct_updt_dte[26];					/* date                                 */
    int     prjct_updt_by_party_nbr;			/* number(11,0)                         */
    int     prjct_stus_cde;						/* number(6,0)			not null enable */
  } prjct;

  /* prjct indicators */
  struct 
  {
    short   prjct_nte_txt;
    short   prjct_updt_dte;
    short   prjct_updt_by_party_nbr;
  } prjct_ind;

  /* prjct_fscl_dtl */
  struct 
  {
    int     prjct_fscl_dtl_nbr;					/* number(11,0)		not null enable		*/
    VARCHAR prjct_fscl_dtl_nte_txt[2001];		/* clob									*/
    int     prjct_fscl_dtl_yr_end_nbr;			/* number(4,0)		not null enable		*/
    int     prjct_nbr;							/* number(11,0)		not null enable		*/
    int     prjct_stus_cde;						/* number(6,0)		not null enable		*/
  } prjct_fscl_dtl;

  /* prjct_fscl_dtl indicators*/
  struct 
  {
    short   prjct_fscl_dtl_nte_txt;
  } prjct_fscl_dtl_ind;


  /* wrk_ordr */
  struct 
  {
    int     wrk_ordr_nbr;						/* number(11,0)		not null enable		*/ 
    int     wrk_ordr_orntr_party_nbr;			/* number(11,0)		not null enable		*/
    char    wrk_ordr_crte_dte[26];				/* date				not null enable		*/
    int     wrk_ordr_crte_by_party_nbr;			/* number(11,0)		not null enable		*/
    char    wrk_ordr_updt_dte[26];				/* date									*/
    int     wrk_ordr_updt_by_party_nbr;			/* number(11,0)							*/
    char    wrk_ordr_req_dte[26];				/* date									*/
    VARCHAR wrk_ordr_nte_txt[2001];				/* clob									*/
    int     parnt_wrk_ordr_nbr;					/* number(11,0)							*/
    int     wrk_ordr_stus_cde;					/* number(6,0)		not null enable		*/
    int     wrk_ordr_mtrl_typ_cde;				/* number(6,0)		not null enable		*/
   } wrk_ordr;

  /* wrk_ordr */
  struct 
  {
    short   wrk_ordr_updt_dte;
    short   wrk_ordr_updt_by_party_nbr;
    short   wrk_ordr_req_dte;
    short   wrk_ordr_nte_txt;
    short   parnt_wrk_ordr_nbr;
   } wrk_ordr_ind;

  struct 
  {
    int     wrk_ordr_clnt_nbr;					/* number(11,0)		not null enable 	*/
    VARCHAR ltr_nbr_txt[65];					/* varchar2(64 byte)					*/
    int     wrk_ordr_clnt_party_nbr;			/* number(11,0)							*/ 
    VARCHAR wrk_ordr_clnt_nte_txt[3001];		/* clob									*/
    int     wrk_ordr_nbr;						/* number(11,0)		not null enable		*/
    int     wrk_ordr_paymt_typ_cde;				/* number(6,0)		not null enable		*/
  } wrk_ordr_clnt;

  /* wrk_ordr_clnt indicators */
  struct 
  {
    short   ltr_nbr_txt;
    short   wrk_ordr_clnt_party_nbr;
    short   wrk_ordr_clnt_nte_txt;
  } wrk_ordr_clnt_ind;


  /* wrk_ordr_fscl_dtl_acs_pnt */
  struct 
  {
    int     wrk_ordr_fscl_dtl_nbr;				/* number(11,0)		not null enable		*/
    int     prjct_fscl_dtl_nbr;					/* number(11,0)		not null enable		*/
    int     wrk_ordr_nbr;						/* number(11,0)		not null enable		*/
  } wrk_ordr_fscl_dtl_acs_pnt;

  /* wrk_ordr_lab */
  struct 
  {
    int     wrk_ordr_lab_nbr;					/* number(11,0)		not null enable		*/
    int     wrk_ordr_lab_seq_nbr;				/* number(6,0)		not null enable		*/
    int     wrk_ordr_nbr;						/* number(11,0)		not null enable		*/
    int     lab_typ_cde;						/* number(6,0)		not null enable		*/
  } wrk_ordr_lab;

  /* wrk_rqstd */
  struct 
  {
    int     wrk_rqstd_nbr						/* number(11,0)		not null enable		*/
    VARCHAR wrk_rqstd_nte_txt[2001];			/* clob									*/
    int     wrk_ordr_nbr						/* number(11,0)		not null enable		*/
    int     wrk_rqstd_typ_cde					/* number(6,0)		not null enable		*/
  ) wrk_rqstd;

  /* wrk_rqstd indicators */
  struct 
  {
    short	wrk_rqstd_nte_txt;
  ) wrk_rqstd ind;
