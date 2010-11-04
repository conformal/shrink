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

#include <string.h>
#include <sys/time.h>

#include <lzo/lzoconf.h>
#include <lzo/lzo1x.h>

#include <shrink.h>

char	*s_algorithm;
int	s_level;
int	(*s_compress)(u_int8_t *, u_int8_t *, size_t, size_t *,
	    struct timeval *);
int	(*s_decompress)(u_int8_t *, u_int8_t *, size_t, size_t *,
	    struct timeval *);
void	*(*s_malloc)(size_t *);

/* null compression */
void *
s_malloc_null(size_t *sz)
{
	if (sz == NULL)
		return (NULL);

	return (malloc(*sz));
}

int
s_compress_null(u_int8_t *src, u_int8_t *dst, size_t len, size_t *comp_sz,
    struct timeval *elapsed)
{
	struct timeval		end, start;

	/* sanity */
	if (comp_sz == NULL)
		return (S_INTEGRITY);
	if (*comp_sz < len)
		return (S_INTEGRITY);

	if (elapsed && gettimeofday(&start, NULL) == -1)
		return (S_LIBC);

	bcopy(src, dst, len);

	if (elapsed) {
		if (gettimeofday(&end, NULL) == -1)
			return (S_LIBC);
		timersub(&end, &start, elapsed);
	}

	*comp_sz = len;

	return (S_OK);
}

int
s_decompress_null(u_int8_t *src, u_int8_t *dst, size_t len, size_t *uncomp_sz,
    struct timeval *elapsed)
{
	struct timeval		end, start;

	/* sanity */
	if (uncomp_sz == NULL)
		return (S_INTEGRITY);
	if (*uncomp_sz < len)
		return (S_INTEGRITY);

	if (elapsed && gettimeofday(&start, NULL) == -1)
		return (S_LIBC);

	bcopy(src, dst, len);

	if (elapsed) {
		if (gettimeofday(&end, NULL) == -1)
			return (S_LIBC);
		timersub(&end, &start, elapsed);
	}

	*uncomp_sz = len;

	return (S_OK);
}

/* LZO */
#define HEAP_ALLOC(var,size) \
    lzo_align_t __LZO_MMODEL var [ ((size) + (sizeof(lzo_align_t) - 1)) / sizeof(lzo_align_t) ]

/*
 * From the LZO FAQ
 *
 * http://www.oberhumer.com/opensource/lzo/lzofaq.php
 *
 * How much can my data expand during compression ?
 * ================================================

 * LZO will expand incompressible data by a little amount.
 * I still haven't computed the exact values, but I suggest using
 * these formulas for a worst-case expansion calculation:

 * Algorithm LZO1, LZO1A, LZO1B, LZO1C, LZO1F, LZO1X, LZO1Y, LZO1Z:
 * ----------------------------------------------------------------
 * output_block_size = input_block_size + (input_block_size / 16) + 64 + 3

 * [This is about 106% for a large block size.]

 * Algorithm LZO2A:
 * ----------------
 * output_block_size = input_block_size + (input_block_size / 8) + 128 + 3
 */
#define LZO_SIZE(s)	(s + (s / 16) + 64 + 3)
static lzo_uint32	s_lzo1x_heapsz;

static int		(*s_lzo1x_compress)(const lzo_bytep, lzo_uint,
			    lzo_bytep, lzo_uintp, lzo_voidp);

void *
s_malloc_lzo(size_t *sz)
{
	void			*p;
	size_t			real_sz;

	if (sz == NULL)
		return (NULL);

	real_sz = LZO_SIZE(*sz);
	p = malloc(real_sz);
	if (p == NULL)
		return (NULL);

	*sz = real_sz;
	return (p);

}

int
s_compress_lzo(u_int8_t *src, u_int8_t *dst, size_t len, size_t *comp_sz,
    struct timeval *elapsed)
{
	struct timeval		end, start;
	HEAP_ALLOC(wrkmem, s_lzo1x_heapsz);

	/* sanity */
	if (comp_sz == NULL)
		return (S_INTEGRITY);
	if (*comp_sz < len)
		return (S_INTEGRITY);

	if (elapsed && gettimeofday(&start, NULL) == -1)
		return (S_LIBC);

	if (s_lzo1x_compress(src, len, dst, comp_sz, wrkmem) != LZO_E_OK)
		return (S_LIB_COMPRESS);

	if (elapsed) {
		if (gettimeofday(&end, NULL) == -1)
			return (S_LIBC);
		timersub(&end, &start, elapsed);
	}

	return (S_OK);
}

int
s_decompress_lzo(u_int8_t *src, u_int8_t *dst, size_t len, size_t *uncomp_sz,
    struct timeval *elapsed)
{
	struct timeval		end, start;

	/* sanity */
	if (uncomp_sz == NULL)
		return (S_INTEGRITY);
	/* allow for incompressible margin */
	if (LZO_SIZE(*uncomp_sz) < len) {
		return (S_INTEGRITY);
	}

	if (elapsed && gettimeofday(&start, NULL) == -1)
		return (S_LIBC);

	if (lzo1x_decompress(src, len, dst, uncomp_sz, NULL) != LZO_E_OK)
		return (S_LIB_COMPRESS);

	if (elapsed) {
		if (gettimeofday(&end, NULL) == -1)
			return (S_LIBC);
		timersub(&end, &start, elapsed);
	}

	return (S_OK);
}
/* LZW */

/* LZMA */

/* init */
int
s_init(int algorithm, int level)
{
	switch (algorithm) {
	case S_ALG_NULL:
		if (level != S_L_NONE)
			return (S_INVALID);

		s_algorithm = "null";
		s_compress = s_compress_null;
		s_decompress = s_decompress_null;
		s_malloc = s_malloc_null;
		s_level = level;
		break;
	case S_ALG_LZO:
		if (lzo_init() != LZO_E_OK)
			return (S_LIB_COMPRESS);
		switch (level) {
		case S_L_MIN:
			s_lzo1x_compress = lzo1x_1_compress;
			s_lzo1x_heapsz = LZO1X_1_MEM_COMPRESS;
			s_algorithm = "lzo1x_1";
			break;
		case S_L_MID:
			s_lzo1x_compress = lzo1x_1_15_compress;
			s_lzo1x_heapsz = LZO1X_1_15_MEM_COMPRESS;
			s_algorithm = "lzo1x_1_15";
			break;
		case S_L_MAX:
			s_lzo1x_compress = lzo1x_999_compress;
			s_lzo1x_heapsz = LZO1X_999_MEM_COMPRESS;
			s_algorithm = "lzo1x_999";
			break;
		case S_L_NONE:
		default:
			return (S_INVALID);
		}
		s_compress = s_compress_lzo;
		s_decompress = s_decompress_lzo;
		s_malloc = s_malloc_lzo;
		s_level = level;
		break;
	default:
		return (S_INVALID);
	}

	return (S_OK);
}
