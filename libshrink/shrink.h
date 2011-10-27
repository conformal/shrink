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

#ifdef NEED_LIBCLENS
#include <clens.h>
#endif

#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>

/* versioning */
#define SHRINK_STRINGIFY(x)	#x
#define SHRINK_STR(x)		SHRINK_STRINGIFY(x)
#define SHRINK_VERSION_MAJOR	0
#define SHRINK_VERSION_MINOR	3
#define SHRINK_VERSION_PATCH	0
#define SHRINK_VERSION		SHRINK_STR(SHRINK_VERSION_MAJOR) "." \
				SHRINK_STR(SHRINK_VERSION_MINOR) "." \
				SHRINK_STR(SHRINK_VERSION_PATCH)

const char	*shrink_verstring(void);
void		 shrink_version(int *major, int *minor, int *patch);

#define SHRINK_OK		(0)
#define SHRINK_INTEGRITY	(1)
#define SHRINK_INVALID		(2)
#define SHRINK_LIBC		(3)
#define SHRINK_LIB_COMPRESS	(4)

#define SHRINK_ALG_NULL		(0)
#define SHRINK_ALG_LZO		(1)
#define SHRINK_ALG_LZW		(2)
#define SHRINK_ALG_LZMA		(3)

#define SHRINK_L_NONE		(0)
#define SHRINK_L_MIN		(1)
#define SHRINK_L_MID		(2)
#define SHRINK_L_MAX		(3)

/* XXX do we want to expose the internal API? */

/* pretty api */
extern int		(*s_compress)(uint8_t *, uint8_t *, size_t, size_t *,
			    struct timeval *);
extern int		(*s_decompress)(uint8_t *, uint8_t *, size_t,
			    size_t *, struct timeval *);
extern void		*(*s_malloc)(size_t *);
extern size_t		(*s_compress_bounds)(size_t);
extern char		*s_algorithm;

int			s_init(int algorithm, int level);
