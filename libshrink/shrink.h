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

#define S_OK		(0)
#define S_INTEGRITY	(1)
#define S_INVALID	(2)
#define S_LIBC		(3)
#define S_LIB_COMPRESS	(4)

#define S_ALG_NULL	(0)
#define S_ALG_LZO	(1)
#define S_ALG_LZW	(2)
#define S_ALG_LZMA	(3)

#define S_L_NONE	(0)
#define S_L_MIN		(1)
#define S_L_MID		(2)
#define S_L_MAX		(3)

#if 0
/* XXX do we want to expose the internal API? */
/* null compression */
int			s_compress_null(u_int8_t *, u_int8_t *, size_t,
			    size_t *, struct timeval *);
int			s_decompress_null(u_int8_t *, u_int8_t *, size_t,
			    size_t *, struct timeval *);
void			*s_malloc_null(size_t *);

/* lzo compression */
int			s_compress_lzo999(u_int8_t *, u_int8_t *, size_t,
			    size_t *, struct timeval *);
int			s_decompress_lzo999(u_int8_t *, u_int8_t *, size_t,
			    size_t *, struct timeval *);
void			*s_malloc_lzo999(size_t *);
#endif

/* pretty api */
extern int		(*s_compress)(u_int8_t *, u_int8_t *, size_t, size_t *,
			    struct timeval *);
extern int		(*s_decompress)(u_int8_t *, u_int8_t *, size_t,
			    size_t *, struct timeval *);
extern void		*(*s_malloc)(size_t *);
extern char		*s_algorithm;

int			s_init(int algorithm, int level);
