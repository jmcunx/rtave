/*
 * Copyright (c) 2021 2022
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
#include <sys/param.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <j_lib2.h>
#include <j_lib2m.h>
#include "rtave.h"

#define SCKARG 80
#define MSG_HELP_11 "Print Average Time Difference"

char *rtave_i_c="$Id: rtave_i.c,v 1.4 2021/04/20 22:18:29 jmccue Exp $";
extern char *rtave_c;


/*
 * init_w() -- initialize work area
 */
void init_w(struct s_work *w)

{

  w->total_records       = (COUNTER) 0;
  w->selected_records    = (COUNTER) 0;
  w->seconds_total_diff  = (COUNTER) 0;
  w->seconds_min         = (time_t) 9999999999;
  w->seconds_max         = (time_t) 0;
  w->seconds_ave         = (time_t) 0;
  w->verbose             = 0;
  w->all                 = FALSE;
  w->out_csv             = FALSE;
  w->show_title          = FALSE;

  memset(w->item_name,JLIB2_CHAR_NULL,(MAX_SIZE_ITEM + 1));

}  /* init_w() */

/*
 * show_rev()
 */
void show_rev(FILE *fp)
{
  fprintf(fp,"%s %s:\n", PROG_NAME, LIT_REV);
  fprintf(fp,"\t%s\n", RTAVE_H);
  fprintf(fp,"\t%s\n", rtave_c);
  fprintf(fp,"\t%s\n", rtave_i_c);


#ifdef J_LIB2M_H
  fprintf(fp, "\t%s\n", J_LIB2M_H);
#endif
#ifdef J_LIB2_H
  fprintf(fp, "\t%s\n", J_LIB2_H);
  fprintf(fp, "\t     %s %s\n", LIT_INFO_02, j2_get_build());
#endif

#ifdef OSTYPE
  fprintf(fp,"\t%s\n",OSTYPE);
#endif  /* OSTYPE  */
#ifdef PRETTY_NAME
  fprintf(fp,"\t%s\n",PRETTY_NAME);
#endif  /* OSTYPE  */
  fprintf(fp, LIT_INFO_01, __DATE__, __TIME__);

  exit(EXIT_FAILURE);

} /* show_rev() */

/*
 * show_brief_help();
 */
void show_brief_help(FILE *fp)
{
  fprintf(fp, USG_MSG_USAGE, PROG_NAME);
  fprintf(fp, "\t%s\n", MSG_HELP_11);
  fprintf(fp, USG_MSG_OPTIONS);
  fprintf(fp, USG_MSG_ARG_ALL_1,     SWITCH_CHAR, ARG_ALL);
  fprintf(fp, USG_MSG_ARG_FMT_CSV,   SWITCH_CHAR, ARG_FMT_CSV);
  fprintf(fp, USG_MSG_ARG_HELP,      SWITCH_CHAR, ARG_HELP);
  fprintf(fp, USG_MSG_ARG_TITLE_2,   SWITCH_CHAR, ARG_TITLE);
  fprintf(fp, USG_MSG_ARG_VERSION,   SWITCH_CHAR, ARG_VERSION);
  fprintf(fp, USG_MSG_ARG_VERBOSE_8, SWITCH_CHAR, ARG_VERBOSE);

  exit(EXIT_FAILURE);

} /* show_brief_help() */

/*
 * process_arg() -- process arguments
 */
void process_arg(int argc, char **argv, struct s_work *w)

{
  char ckarg[SCKARG];
  int opt;

  snprintf(ckarg, SCKARG, "%c%c%c%c%c%c", 
	   ARG_ALL, ARG_FMT_CSV, ARG_HELP, ARG_TITLE,
           ARG_VERBOSE, ARG_VERSION);

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	case ARG_ALL:
	  w->all = TRUE;
	  break;
	case ARG_FMT_CSV:
	  w->out_csv = TRUE;
	  break;
	case ARG_HELP:
	  show_brief_help(stderr);
	  break;
	case ARG_TITLE:
	  w->show_title = TRUE;
	  break;
	case ARG_VERBOSE:
	  w->verbose++;
	  break;
	case ARG_VERSION:
	  show_rev(stderr);
	  break;
	default:
	  fprintf(stderr, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
	  exit(EXIT_FAILURE);
	  break;
	}
    }

} /* END process_arg() */

/*
 * init() -- initialize
 */
void init(int argc, char **argv, struct s_work *w)

{

  init_w(w);

  process_arg(argc, argv, w);

}  /* init() */

/* END: aver_i.c */
