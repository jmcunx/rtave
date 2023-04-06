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
#include <sys/param.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "rtave.h"

#define SCKARG 80
#define MSG_HELP_11 "Print Average Time Difference"

/*
 * set_fname() -- save a file name, adjust for stdin
 */
void set_fname(struct s_file *f, char *fname)
{

  if (f->fname != (char *) NULL)
    {
      free(f->fname);
      f->fname = (char *) NULL;
    }

  if (fname == (char *) NULL)
    return; /* assume stdin, stdout, stderr */
  if (strlen(fname) == 0)
    return; /* assume stdin, stdout, stderr */
  if (strcmp(fname, FILE_NAME_STDIN) == 0)
    return; /* assume stdin, stdout, stderr */

  f->fname = strdup(fname);
      
} /* set_fname() */

/*
 * init_file() -- clear s_file
 */
void init_file(struct s_file *f)

{
  f->fname       = (char *) NULL;
  f->fp          = (FILE *) NULL;
  f->allow_close = FALSE;
}

/*
 * open_write()
 */
void open_write(struct s_file *f, int force, int is_stderr)
{

  if (f->fname == (char *) NULL)
    {
      if (is_stderr == TRUE)
	f->fp = stderr;
      else
	f->fp = stdout;
      f->allow_close = FALSE;
      return;
    }

  if (force == FALSE)
    {
      if (j2_f_exist(f->fname) == FALSE)
	{
	  fprintf(stderr, MSG_ERR_E025, f->fname);
	  exit(EXIT_FAILURE);
	}
    }

  f->fp = fopen(f->fname, "w");

  if (f->fp == (FILE *) NULL)
    {
      fprintf(stderr, MSG_ERR_E002, f->fname);
      fprintf(stderr, "    %s\n", strerror(errno));
      exit(EXIT_FAILURE);
    }
  
  f->allow_close = TRUE;

} /* open_write() */

/*
 * open_read()
 */
int open_read(struct s_file *f, struct s_file *e, char *fname)
{

  set_fname(f, fname);
  
  if (f->fname == (char *) NULL)
    {
      f->fp = stdin;
      f->allow_close = FALSE;
      return(TRUE);
    }

  f->fp = fopen(f->fname, "r");
  if (f->fp == (FILE *) NULL)
    {
      fprintf(e->fp, MSG_ERR_E001, f->fname);
      fprintf(e->fp, "    %s\n", strerror(errno));
      return(FALSE);
    }
  
  f->allow_close = TRUE;
  return(TRUE);

} /* open_read() */

/*
 * close_file()
 */
void close_file(struct s_file *f)
{
  if (f->allow_close == TRUE)
    fclose(f->fp);

  if (f->fname != (char *) NULL)
    {
      free(f->fname);
      f->fname = (char *) NULL;
    }

  f->allow_close = FALSE;
  f->fp = (FILE *) NULL;
  
} /* close_file() */

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
  w->force               = FALSE;

  init_file(&(w->in));
  init_file(&(w->out));
  init_file(&(w->err));
  memset(w->item_name,JLIB2_CHAR_NULL,(MAX_SIZE_ITEM + 1));

}  /* init_w() */

/*
 * show_rev()
 */
void show_rev(FILE *fp)
{
  fprintf(fp,"%s %s:\n", PROG_NAME, LIT_REV);

#ifdef J_LIB2_H
  fprintf(fp, "\t%s %s\n", LIT_INFO_02, j2_get_build());
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
  fprintf(fp, USG_MSG_ARG_ERR,       SWITCH_CHAR, ARG_ERR);
  fprintf(fp, USG_MSG_ARG_FORCE,     SWITCH_CHAR, ARG_FORCE);
  fprintf(fp, USG_MSG_ARG_HELP,      SWITCH_CHAR, ARG_HELP);
  fprintf(fp, USG_MSG_ARG_OUT,       SWITCH_CHAR, ARG_OUT);
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

  snprintf(ckarg, SCKARG, "%c%c%c%c%c%c%c%c:%c:", 
	   ARG_ALL, ARG_FMT_CSV, ARG_HELP, ARG_TITLE,
           ARG_VERBOSE, ARG_VERSION, ARG_FORCE,
           ARG_OUT, ARG_ERR);

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	case ARG_ERR:
	  if (w->err.fname == (char *) NULL)
	    set_fname(&(w->err), optarg);
	  else
	    {
	      fprintf(stderr, MSG_ERR_E074, SWITCH_CHAR, ARG_OUT);
	      fprintf(stderr, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
	      exit(EXIT_FAILURE);
	    }
	  break;
	case ARG_OUT:
	  if (w->out.fname == (char *) NULL)
	    set_fname(&(w->out), optarg);
	  else
	    {
	      fprintf(stderr, MSG_ERR_E074, SWITCH_CHAR, ARG_OUT);
	      fprintf(stderr, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
	      exit(EXIT_FAILURE);
	    }
	  break;
	case ARG_FORCE:
	  w->force = TRUE;
	  break;
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

  open_write(&(w->err), w->force, TRUE);
  open_write(&(w->out), w->force, FALSE);

}  /* init() */
