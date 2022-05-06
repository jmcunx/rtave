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

#ifndef RTAVE_H
#define RTAVE_H "INCLUDED"

#define MIN_REC 28
#define MAX_SIZE_ITEM 512

#define PROG_NAME "rtave"
#define COUNTER  long int
#define QTY_TYPE long double

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

/*** prototypes ***/
void init(int argc, char **argv, struct s_work *w);
void init_file(struct s_file *f);
void set_fname(struct s_file *f, char *fname);
int open_read(struct s_file *f, struct s_file *e, char *fname);
void open_write(struct s_file *f, int force, int is_stderr);
void close_file(struct s_file *f);

#endif /* RTAVE_H */
