/*
 * Copyright (c) 2021 2022 2023 2024
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef RTAVE_H
#define RTAVE_H "INCLUDED"

#define MIN_REC 28
#define MAX_SIZE_ITEM 512

#define PROG_NAME "rtave"
#define COUNTER  long int
#define QTY_TYPE long double

#ifndef JLIB2_CHAR_NULL
#define NO_JLIB 1
#define JLIB2_INT_NULL   ((int) '\0')
#define JLIB2_CHAR_NULL  ((char) '\0')
#define JLIB2_UCHAR_NULL ((unsigned char) '\0' )
#endif

#ifndef NULL
#define NULL '\0'
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef TAB_CHAR
#define TAB_CHAR 0x09
#endif

#ifdef NO_JLIB
#define J2_CANT_GET      0
#define J2_UNKNOWN       1
#define J2_DIRECTORY     2      /* S_IFDIR  */
#define J2_REGULAR       3      /* S_IFREG  */
#define J2_CHAR_SPECIAL  4      /* S_IFCHR  */
#define J2_BLOCK_SPECIAL 5      /* S_IFBLK  */
#define J2_PIPE          6      /* S_IFPIP  */
#define J2_SOCKET        7      /* S_IFSOCK */
#define J2_SYMB_LINK     8      /* S_IFLNK  */
#define J2_FIFO          9      /* S_IFIFO  */
#define SECS_DAY         86400L /* Seconds per day */
#define SIZ_YYYY         5
#define SIZ_YY           3
#define DATE_SIZE_MAX    27     /* largest size of a date string              */
#define DATE_IN_FMT_E    1      /* in date format DD/MM/YY or DD/MM/YYYY      */
#define DATE_IN_FMT_U    2      /* in date format MM/DD/YY or MM/DD/YYYY      */
#define DATE_IN_FMT_J    3      /* in date format YY/MM/DD or YYYY/MM/DD      */
#define DATE_IN_FMT_NOT  4      /* in date not formatted, YYYYMMDD YYMMDD ... */
#define LIMIT_TO_SEC_YY  9999
#define LIMIT_TO_SEC_MM  12
#define LIMIT_TO_SEC_DD  31
#endif /* NO_JLIB */

#ifdef _MSDOS
#ifndef SNPRINTF
#define SNPRINTF _snprintf
#endif
#ifndef SSIZE_T
#define SSIZE_T unsigned long int
#endif
#endif /* _MSDOS */

#ifndef CUTOFF_YY
#define CUTOFF_YY           90  /* if less than this year use CENTURY_21 */
#endif
#ifndef CENTURY_CURRENT
#define CENTURY_CURRENT     19
#endif
#ifndef CENTURY_NEXT
#define CENTURY_NEXT        20
#endif

#ifndef TIME_SEPERATOR
#  define TIME_SEPERATOR ':'
#endif
#ifndef DATE_SEPERATOR
#  define DATE_SEPERATOR '/'
#endif
#ifndef SNPRINTF
#define SNPRINTF snprintf
#endif
#ifndef SSIZE_T
#define SSIZE_T ssize_t
#endif

#ifdef NO_JLIB
struct s_j2_datetime
{
  int month;    /* Month,           01 --12             */
  int dd;       /* Day of Month,    01 -- [28|29|30|31] */
  int yy;       /* two digit year,  00 -- 99            */
  int yyyy;     /* four digit year, 0000 -- 9999        */
  int hh;       /* Hour of the day, 00 -- 23            */
  int minutes;  /* minutes,         00 -- 59            */
  int ss;       /* seconds,         00 -- 59            */
  int mil;      /* milliseconds,    000 -- 999          */
  int tm_isdst; /* Daylight ?  0 = no, > 0 = yes        */
} ;
#endif /* NO_JLIB */

struct s_file
{
  FILE *fp;
  char *fname;
  int allow_close;
} ;

struct s_rec
{
  struct s_j2_datetime start_date;
  struct s_j2_datetime end_date;
  time_t seconds_start;
  time_t seconds_end;
  time_t seconds_diff;
  char item_name[(MAX_SIZE_ITEM + 1)];
  COUNTER recnum;
} ;

struct s_work
{
  int verbose;
  int all;
  int out_csv;
  int show_title;
  int force;
  char item_name[(MAX_SIZE_ITEM + 1)];
  COUNTER total_records;
  COUNTER selected_records;
  COUNTER seconds_total_diff;
  time_t  seconds_min;
  time_t  seconds_max;
  time_t  seconds_ave;
  struct s_file in;
  struct s_file out;
  struct s_file err;
} ;

/*** messages ***/
#ifdef NO_JLIB
#define ARG_ERR           'e'  /* Output Error File                  */
#define ARG_FMT_CSV       'c'  /* Format output as CSV               */
#define ARG_FORCE         'f'  /* force create files                 */
#define FILE_NAME_STDIN  "-"
#define LIT_STDIN         "(standard input)"
#define MSG_ERR_E086    "ERROR E086: Rec %ld - Start Date greater than End Date, see\n"
#define MSG_ERR_E087    "ERROR E087: Rec %ld - Invalid Date/Time found '%s'\n"
#define MSG_INFO_I005 "I005: Reads %-ld : File: %s\n"
#define MSG_INFO_I051 "I051: For Record Number: %ld\n"
#define MSG_INFO_I052 "I052: %10s %02d/%02d/%04d %02d:%02d:%02d %ld\n"
#define MSG_WARN_W029 "W029: Rec %ld - outlier, bypassing\n"
#define MSG_WARN_W030 "W030: Bypassed Record %ld, invalid format\n"
#define SWITCH_CHAR   '-'
#define USG_MSG_ARG_ERR                  "\t%c%c file\t\t: Write errors to file 'file', default stderr\n"
#define USG_MSG_ARG_FMT_CSV              "\t%c%c\t\t: Format output as a csv file\n"
#define USG_MSG_ARG_FORCE                "\t%c%c\t\t: force create of files when found\n"
#define ARG_VERBOSE       'v'  /* Verbose                            */
#define USG_MSG_ARG_VERBOSE_8            "\t%c%c\t\t: verbose level, each time specified level increases\n"
#define ARG_VERSION       'V'  /* Show Version Information           */
#define USG_MSG_ARG_VERSION              "\t%c%c\t\t: Show revision information and exit\n"
#define ARG_OUT           'o'  /* Output File                        */
#define USG_MSG_ARG_OUT                  "\t%c%c file\t\t: Write output to file 'file', default stdout\n"
#define ARG_HELP       'h'  /* Show Help                      */
#define USG_MSG_ARG_HELP                 "\t%c%c\t\t: Show brief help and exit\n"
#define ARG_ALL           'a'  /* Turn on all options                */
#define USG_MSG_ARG_ALL                  "\t%c%c\t\t: Turn all displays on\n"
#define MSG_ERR_E000    "Try '%s %c%c' for more information\n"
#define MSG_ERR_E074    "ERROR E074: 'Too many Arguments specified for %c%c\n"
#define ARG_TITLE         't'  /* Report Title                       */
#define USG_MSG_ARG_TITLE_2              "\t%c%c\t\t: Print Column Headings, default No Headings\n"
#define USG_MSG_ARG_ALL_1                "\t%c%c\t\t: Process all Valid Records\n"
#define USG_MSG_OPTIONS                  "Options\n"
#define USG_MSG_USAGE                    "usage:\t%s [OPTIONS] [FILES ...]\n"
#define LIT_INFO_01  "\tBuild: %s %s\n"
#define LIT_REV           "Revision"
#define MSG_ERR_E001    "ERROR E001: Cannot open '%s' for read, file processing bypassed\n"
#define MSG_ERR_E002    "ERROR E002: Cannot open '%s' for write, processing aborted\n"
#define MSG_ERR_E025    "ERROR E025: File %s cannot be created, already exists\n"
#endif /* NO_JLIB */

/*** prototypes ***/
void init(int argc, char **argv, struct s_work *w);
void init_file(struct s_file *f);
void set_fname(struct s_file *f, char *fname);
int open_read(struct s_file *f, struct s_file *e, char *fname);
void open_write(struct s_file *f, int force, int is_stderr);
void close_file(struct s_file *f);

#ifdef NO_JLIB
SSIZE_T j2_getline(char **buf, size_t *n, FILE *fp);
int j2_d_get_yyyy(int yy);
int j2_date_is_valid(struct s_j2_datetime *d);
int j2_date_split(struct s_j2_datetime *d, char *d_string, int fmt);
int j2_date_split_fmt(struct s_j2_datetime *d, char *dstr, char *delim, int date_type);
int j2_ds_split(struct s_j2_datetime *d, char *date_time, int fmt);
int j2_f_exist(char *file_name);
int j2_is_numr(char *s);
int j2_time_split(struct s_j2_datetime *d, char *t_string);
long int j2_clr_str(char *s, char c, int size);
long int j2_d_to_sec(struct s_j2_datetime *d);
long int j2_justleft(char *s);
long int j2_rtw(char *buffer);
time_t j2_count_days(int year, int month, int day);
time_t j2_count_years(int year);
void j2_d_init(struct s_j2_datetime *d);
void j2_ds_date_time(char **date_part, char **time_part, char *date_time);
int j2_d_isleap(int year_yyyy);
#endif /* NO_JLIB */

#endif /* RTAVE_H */
