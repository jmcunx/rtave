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

#ifndef _WIN32
#ifndef _MSDOS
#include <sys/param.h>
#endif
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "rtave.h"

/*
 * init_rec()
 */
void init_rec(struct s_rec *r)
{
  j2_d_init(&(r->start_date));
  j2_d_init(&(r->end_date));

  r->seconds_start = (time_t) 0;
  r->seconds_end   = (time_t) 0;
  r->seconds_diff  = (time_t) 0;
  r->recnum        = (COUNTER) 0;
  memset(r->item_name,JLIB2_CHAR_NULL,(MAX_SIZE_ITEM + 1));
  
} /* init_rec() */

/*
 * show_dts() - show date/time info
 */
void show_dts(struct s_file *err, struct s_j2_datetime dfmt,
	      time_t seconds, char *title)
{
  fprintf(err->fp, MSG_INFO_I052, title,
	  dfmt.month, dfmt.dd, dfmt.yyyy,
	  dfmt.hh, dfmt.minutes, dfmt.ss,
	  (long) seconds);
} /* show_dts() */

/*
 * get_seconds() - Show results
 */
char *get_seconds(time_t secs)
{
  static char fmt_secs[50];
  COUNTER h = (COUNTER) 0;
  COUNTER m = (COUNTER) 0;
  COUNTER s = (COUNTER) 0;
  COUNTER d = (COUNTER) 0;

  m = secs / 60;
  s = secs % 60;
  if (m > 60)
    {
      h = m / 60;
      m = m - (60 * h);
    }
  if (h > 23)
    {
      d = h / 24;
      h = h - (24 * d);
    }

  memset(fmt_secs, JLIB2_CHAR_NULL, 50);

  snprintf(fmt_secs, 49, "%4ldd %02ld:%02ld:%02ld", d,h,m,s);
  j2_rtw(fmt_secs);
  j2_justleft(fmt_secs);

  return(fmt_secs);
  
} /* get_seconds() */

/*
 * show_seconds() - Show results
 */
void show_seconds(FILE *fp, time_t secs, char *title)
{
     
  fprintf(fp, "%-20s: %11ld =~ %s\n",
	  title, (long) secs, get_seconds(secs));
  
} /* END: show_seconds() */

/*
 * show_results() - Show results
 */
void show_results(FILE *fp, struct s_work *w)
{
  static int show_hd=TRUE;

  if ( w->out_csv == TRUE)
    {
      if (show_hd == TRUE)
	{
	  show_hd=FALSE;
	  if (w->show_title == TRUE)
	    {
	      fprintf(fp, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\"\n",
		      "Entry",
		      "Total Records",
		      "Selected Records",
		      "Bypassed Records",
		      "Total Run Time",
		      "Minimum Run Time",
		      "Maximum Run Rime",
		      "Average Run Time");
	    }
	  
	}
	  fprintf(fp, "\"%s\",%ld,%ld,%ld,",
		  w->item_name,
		  w->total_records,
		  w->selected_records,
		  (w->total_records - w->selected_records));
	  fprintf(fp, "\"%s\",",  get_seconds(w->seconds_total_diff));
	  fprintf(fp, "\"%s\",",  get_seconds(w->seconds_min));
	  fprintf(fp, "\"%s\",",  get_seconds(w->seconds_max));
	  fprintf(fp, "\"%s\"\n", get_seconds(w->seconds_ave));
    }
  else
    {
      if (w->verbose > 0)
	{
	  fprintf(fp, "%-20s: %ld\n", "Total Records", w->total_records);
	  fprintf(fp, "%-20s: %ld\n", "Selected Records", w->selected_records);
	  if (w->total_records != w->selected_records)
	    fprintf(fp, "%-20s: %ld\n", "Bypassed Records",
		    (w->total_records - w->selected_records));
	  fprintf(fp, "%-20s: %11ld =~ %s\n",
		  "Total Seconds",
		  w->seconds_total_diff,
		  get_seconds(w->seconds_total_diff));
	  fprintf(fp, "%-20s: %11ld =~ %s\n",
		  "Minimum Seconds",
		  (long) w->seconds_min,
		  get_seconds(w->seconds_min));
	  fprintf(fp, "%-20s: %11ld =~ %s\n",
		  "Maximum Seconds",
		  (long) w->seconds_max,
		  get_seconds(w->seconds_max));
	}
      fprintf(fp, "%-20s: %11ld =~ %s\n",
	      "Average Seconds",
	      (long) w->seconds_ave,
	      get_seconds(w->seconds_ave));
    }

} /* show_results() */

/*
 * load_date()
 */
void load_date(struct s_file *err, struct s_j2_datetime *dfmt, char *buf, COUNTER recnum, int idx)
{
  char dt[DATE_SIZE_MAX];

  j2_d_init(dfmt);
  memset(dt,JLIB2_CHAR_NULL,DATE_SIZE_MAX);

  strncpy(dt, &(buf[idx]), 14);

  if ( ! j2_is_numr(dt) )
    {
      fprintf(err->fp, MSG_ERR_E087, recnum, dt);
      exit(EXIT_FAILURE);
    }

  /*** manual needs to be fixed ***/
  j2_ds_split(dfmt, dt, DATE_IN_FMT_NOT);

  if ( ! j2_date_is_valid(dfmt) )
    {
      fprintf(err->fp, MSG_ERR_E087, recnum, dt);
      exit(EXIT_FAILURE);
    }
  
} /* load_date() */

/*
 * load_rec()
 */
int load_rec(struct s_file *err, struct s_rec *r, char *buf, COUNTER recnum, int verbose, int all)
{

  size_t sbuf = 0;

  init_rec(r);

  if (buf == (char *) NULL)
    {
      if (verbose > 1)
	fprintf(err->fp, MSG_WARN_W030, recnum);
      return(FALSE);
    }

  sbuf = strlen(buf);

  if (sbuf < MIN_REC)
    {
      if (verbose > 1)
	fprintf(err->fp, MSG_WARN_W030, recnum);
      return(FALSE);
    }

  if (sbuf > MIN_REC)
    strncpy(r->item_name, &(buf[28]), MAX_SIZE_ITEM);

  load_date(err,&(r->start_date),buf,recnum,0);
  load_date(err,&(r->end_date),buf,recnum,14); 
  r->recnum = recnum;

  r->seconds_start = j2_d_to_sec(&(r->start_date));
  r->seconds_end   = j2_d_to_sec(&(r->end_date));

  if (r->seconds_start > r->seconds_end)
    {
      fprintf(err->fp, MSG_ERR_E086, r->recnum);
      show_dts(err, r->start_date, r->seconds_start, "START");
      show_dts(err, r->end_date,   r->seconds_end,   "END");
      exit(EXIT_FAILURE);
    }

  r->seconds_diff = r->seconds_end - r->seconds_start;

  if (r->seconds_diff < (time_t) SECS_DAY)
    return(TRUE);
  if (all == TRUE)
    return(TRUE);

  if (verbose > 1)
    {
      fprintf(err->fp, MSG_WARN_W029, r->recnum);
      show_dts(err, r->start_date, r->seconds_start, "START");
      show_dts(err, r->end_date,   r->seconds_end,   "END");
    }
  
  return(FALSE);
  
} /* load_rec() */

/*
 * process_a_file()
 */
void process_a_file(struct s_work *w, char *fname, char **buf, size_t *bsize)
{
  long int reads = 0L;
  struct s_rec rec;

  if (open_read(&(w->in), &(w->err), fname) != TRUE)
    exit(EXIT_FAILURE);

  while (j2_getline(buf, bsize, w->in.fp) > (ssize_t) -1)
    {
      reads++;
      w->total_records++;
      j2_rtw((*buf));
      if (load_rec(&(w->err), &rec, (*buf), w->total_records, w->verbose, w->all) != TRUE)
	continue;
      if (*w->item_name == JLIB2_CHAR_NULL)
	{
	  if (strlen(rec.item_name) > 0)
	    strncpy(w->item_name, rec.item_name, MAX_SIZE_ITEM);
	}

      w->selected_records++;
      w->seconds_total_diff += rec.seconds_diff;
      if (w->seconds_min > rec.seconds_diff)
	w->seconds_min = rec.seconds_diff;
      if (w->seconds_max < rec.seconds_diff)
	w->seconds_max = rec.seconds_diff;
      if (w->verbose > 2)
	{
	  fprintf(w->err.fp, MSG_INFO_I051, reads);
	  show_dts(&(w->err), rec.start_date, rec.seconds_start, "START");
	  show_dts(&(w->err), rec.end_date,   rec.seconds_end,   "END");
	}
    }

  if (w->verbose > 0)
    {
      fprintf(w->err.fp, MSG_INFO_I005, reads, 
              (w->in.fname == (char *) NULL ? LIT_STDIN : w->in.fname));
    }

  close_file(&(w->in));

} /* process_a_file() */

/*
 * main()
 */
int main(int argc, char **argv)
{
  struct s_work w;
  char *buf = (char *) NULL;
  size_t bsize = (size_t) 0;
  int i = 0;
  
  init(argc, argv, &w);

  for (i = optind; i < argc; i++)
    process_a_file(&w, argv[i], &buf, &bsize);
  if (i == optind)
    process_a_file(&w, FILE_NAME_STDIN, &buf, &bsize);

  w.seconds_ave = w.seconds_total_diff / w.selected_records;
  show_results(w.out.fp, &w);

#ifdef OpenBSD
  freezero(buf, bsize);
#else
  if (buf != (char *) NULL)
    free(buf);
#endif

  close_file(&(w.out));
  close_file(&(w.err));
  
  /* done */
  exit(EXIT_SUCCESS);

} /* main() */
