/* $shrink$ */
/*
 * Copyright (c) 2010 Marco Peereboom <marco@peereboom.us>
 * Copyright (c) 2010 Conformal Systems LLC <info@conformal.com>
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

#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>
#include <util.h>

#include <shrink.h>

size_t			bs = 10 * 1024 * 1024;
int			count = 1, random_data = 0;

void
print_time_scaled(char *s, struct timeval *t)
{
	int			f = 3;
	double			te;
	char			*scale = "us";

	te = ((double)t->tv_sec * 1000000) + t->tv_usec;
	if (te > 1000) {
		te /= 1000;
		scale = "ms";
	}
	if (te > 1000) {
		te /= 1000;
		scale = "s";
	}

	printf("%s%12.*f%-2s\n", s, f, te, scale);
}

void
print_throughput(char *s, off_t tot_sz, struct timeval *t)
{
	double			us;
	char			human[64];

	us = ((double)t->tv_sec * 1000000.0) + t->tv_usec;
	if (us == 0)
		us = 1;

	fmt_scaled(((double)tot_sz * 1000000.0) / us, human);

	printf("%s%13sB/s\n", s, human);
}

void
print_size(char *s, size_t sz)
{
	char			human[64];

	fmt_scaled(sz, human);

	printf("%s%13sB\n", s, human);
}

void
test_run(int algo, int level)
{
	struct timeval		elapsed, tot_comp, tot_uncomp;
	u_int8_t		*s = NULL, *d = NULL, *uncomp = NULL;
	size_t			tot_comp_sz = 0, tot_uncomp_sz = 0, dsz;
	size_t			uncomp_sz, comp_sz;
	int			i;

	timerclear(&tot_comp);
	timerclear(&tot_uncomp);

	if (s_init(algo, level))
		errx(1, "s_init");

	s = malloc(bs);
	if (s == NULL)
		err(1, "malloc s");
	dsz = bs;
	d = s_malloc(&dsz);
	if (d == NULL)
		err(1, "malloc d");
	uncomp = malloc(bs);
	if (uncomp == NULL)
		err(1, "malloc uncomp");

	for (i = 0; i < count; i++) {
		if (random_data)
			arc4random_buf(s, bs);
		else
			memset(s, i, bs);

		/* compress */
		comp_sz = bs;
		if (s_compress(s, d, bs, &comp_sz, &elapsed))
			errx(1, "s_compress");
		timeradd(&elapsed, &tot_comp, &tot_comp);
		tot_comp_sz += comp_sz;

		/* decompress */
		uncomp_sz = bs;
		if (s_decompress(d, uncomp, comp_sz, &uncomp_sz, &elapsed))
			errx(1, "s_decompress");
		timeradd(&elapsed, &tot_uncomp, &tot_uncomp);
		tot_uncomp_sz += uncomp_sz;

		/* validate */
		if (bcmp(s, uncomp, bs))
			errx(1, "data corruption");
	}

	printf           ("algorithm                    : %12s\n", s_algorithm);
	print_size       ("data size                    : ", bs * count);
	print_size       ("size compressed              : ", tot_comp_sz);
	print_time_scaled("compression                  : ", &tot_comp);
	print_throughput( "compression throughput       : ", bs * count, &tot_comp);
	print_time_scaled("decompression                : ", &tot_uncomp);
	print_throughput( "decompression throughput     : ", bs * count, &tot_uncomp);

	free(s);
	free(d);
	free(uncomp);
}

int
main(int argc, char *argv[])
{
	int			c;

	while ((c = getopt(argc, argv, "b:c:r")) != -1) {
		switch (c) {
		case 'b': /* block size */
			bs = atoi(optarg);
			if (bs <= 0 || bs > 512 * 1024 * 1024)
				errx(1, "invalid block size");
			break;
		case 'c': /* count */
			count = atoi(optarg);
			if (count <= 0 || count > 1000000)
				errx(1, "invalid count");
			break;
		case 'r':
			random_data = 1;
			break;
		default:
			errx(1, "invalid option");
		}
	}

	test_run(S_ALG_NULL, S_L_NONE);
	printf("\n");
	test_run(S_ALG_LZO, S_L_MIN);
	printf("\n");
	test_run(S_ALG_LZO, S_L_MID);
	printf("\n");
	test_run(S_ALG_LZO, S_L_MAX);

	return (0);
}
