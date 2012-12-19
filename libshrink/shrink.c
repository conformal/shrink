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

#if defined(SUPPORT_LZO2)
#include <lzo/lzoconf.h>
#include <lzo/lzo1x.h>
#endif /* SUPPORT LZO2 */

#if defined(SUPPORT_LZW)
#include<zlib.h>
#endif /* SUPPORT LZW */

#if defined(SUPPORT_LZMA)
#include <lzma.h>
#endif /* SUPPORT LZMA */

#include <shrink.h>

#ifdef BUILDSTR
static const char *vertag = SHRINK_VERSION " " BUILDSTR;
#else
static const char *vertag = SHRINK_VERSION;
#endif

struct shrink_ctx {
	char	*s_algorithm;
	int	s_level;
	int	(*s_compress)(struct shrink_ctx *, uint8_t *, uint8_t *,
		    size_t, size_t *);
	int	(*s_decompress)(struct shrink_ctx *, uint8_t *, uint8_t *,
		    size_t, size_t *);
	size_t	(*s_compress_bounds)(struct shrink_ctx *, size_t);
#if defined(SUPPORT_LZO2)
	lzo_uint32	s_lzo1x_heapsz;

	int		(*s_lzo1x_compress)(const lzo_bytep, lzo_uint,
			    lzo_bytep, lzo_uintp, lzo_voidp);
#endif /* defined(SUPPORT_lZO2) */
};

const char *
shrink_verstring(void)
{
	return (vertag);
}

void
shrink_version(int *major, int *minor, int *patch)
{
	*major = SHRINK_VERSION_MAJOR;
	*minor = SHRINK_VERSION_MINOR;
	*patch = SHRINK_VERSION_PATCH;
}

/* null compression */
size_t
s_compress_bounds_null(struct shrink_ctx *ctx, size_t sz)
{
	return (sz);
}

int
s_compress_null(struct shrink_ctx *ctx, uint8_t *src, uint8_t *dst, size_t len,
    size_t *comp_sz)
{
	bcopy(src, dst, len);

	*comp_sz = len;

	return (SHRINK_OK);
}

int
s_decompress_null(struct shrink_ctx *ctx, uint8_t *src, uint8_t *dst,
    size_t len, size_t *uncomp_sz)
{
	bcopy(src, dst, len);

	*uncomp_sz = len;

	return (SHRINK_OK);
}

#if defined(SUPPORT_LZO2)
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

size_t
s_compress_bounds_lzo(struct shrink_ctx *ctx, size_t sz)
{
	return (LZO_SIZE(sz));
}

int
s_compress_lzo(struct shrink_ctx *ctx,  uint8_t *src, uint8_t *dst, size_t len,
    size_t *comp_sz)
{
	HEAP_ALLOC(wrkmem, ctx->s_lzo1x_heapsz);

	/*
	 * In order to guarantee that the compressed buffer is always
	 * identical one has to clear wrkmem.  This is per the O in LZO.
	 */
	bzero(wrkmem, sizeof(wrkmem));
	if (ctx->s_lzo1x_compress(src, len, dst, (lzo_uintp)comp_sz,
	    wrkmem) != LZO_E_OK)
		return (SHRINK_LIB_COMPRESS);
	return (SHRINK_OK);
}

int
s_decompress_lzo(struct shrink_ctx *ctx, uint8_t *src, uint8_t *dst,
    size_t len, size_t *uncomp_sz)
{
	if (lzo1x_decompress(src, len, dst, (lzo_uintp)uncomp_sz,
	    NULL) != LZO_E_OK)
		return (SHRINK_LIB_COMPRESS);
	return (SHRINK_OK);
}
#endif /* SUPPORT_LZO2 */

#if defined(SUPPORT_LZW)
/* LZW */
size_t
s_compress_bounds_lzw(struct shrink_ctx *ctx, size_t sz)
{
	return (compressBound(sz));
}

int
s_compress_lzw(struct shrink_ctx *ctx, uint8_t *src, uint8_t *dst, size_t len,
    size_t *comp_sz)
{
	if (compress2(dst, (uLongf *)comp_sz, src, len,
	   ctx->s_level) != Z_OK)
		return (SHRINK_LIB_COMPRESS);
	return (SHRINK_OK);
}

int
s_decompress_lzw(struct shrink_ctx *ctx, uint8_t *src, uint8_t *dst,
    size_t len, size_t *uncomp_sz)
{

	if (uncompress(dst, (uLongf *) uncomp_sz, src, len) != Z_OK)
		return (SHRINK_LIB_COMPRESS);
	return (SHRINK_OK);
}
#endif /* SUPPORT_LZW */

#if defined(SUPPORT_LZMA)
/* LZMA */
/* XXX pulled out of my butt */
#define LZMA_SIZE(s)	(s + (lzma_block_buffer_bound(s) - s) * 2)

size_t
s_compress_bounds_lzma(struct shrink_ctx *ctx, size_t sz)
{
	return (LZMA_SIZE(sz));
}

int
s_compress_lzma(struct shrink_ctx *ctx, uint8_t *src, uint8_t *dst,
    size_t len, size_t *comp_sz)
{
	lzma_stream		lzma = LZMA_STREAM_INIT;
	int			r;

	lzma.next_in = src;
	lzma.next_out = dst;
	lzma.avail_in = len;
	lzma.avail_out = *comp_sz;
	if (lzma_easy_encoder(&lzma, ctx->s_level,
	    LZMA_CHECK_CRC32) != LZMA_OK) {
		lzma_end(&lzma);
		return (SHRINK_LIB_COMPRESS);
	}
	if (lzma_code(&lzma, LZMA_RUN) != LZMA_OK) {
		lzma_end(&lzma);
		return (SHRINK_LIB_COMPRESS);
	}
	r = lzma_code(&lzma, LZMA_FINISH);
	if (r != LZMA_STREAM_END && r != LZMA_OK) {
		lzma_end(&lzma);
		return (SHRINK_LIB_COMPRESS);
	}
	*comp_sz = lzma.total_out;
	lzma_end(&lzma);

	return (SHRINK_OK);
}

int
s_decompress_lzma(struct shrink_ctx *ctx, uint8_t *src, uint8_t *dst,
    size_t len, size_t *uncomp_sz)
{
	lzma_stream		lzma = LZMA_STREAM_INIT;
	int			r;

	/* sanity */
	if (LZMA_SIZE(*uncomp_sz) < len) {
		return (SHRINK_INTEGRITY);
	}

	lzma.next_in = src;
	lzma.next_out = dst;
	lzma.avail_in = len;
	lzma.avail_out = *uncomp_sz;
	if ((r = lzma_auto_decoder(&lzma,
	    lzma_easy_decoder_memusage(ctx->s_level), 0)) != LZMA_OK) {
		lzma_end(&lzma);
		return (SHRINK_LIB_COMPRESS);
	}
	r = lzma_code(&lzma, LZMA_RUN);
	if (r != LZMA_STREAM_END) {
		lzma_end(&lzma);
		return (SHRINK_LIB_COMPRESS);
	}
	*uncomp_sz = lzma.total_out;
	lzma_end(&lzma);

	return (SHRINK_OK);
}
#endif /* SUPPORT_LZMA */

struct shrink_ctx *
shrink_init(int algorithm, int level)
{
	struct shrink_ctx	*ctx;

	if ((ctx = calloc(1, sizeof(*ctx))) == NULL)
		return (ctx);

	switch (algorithm) {
	case SHRINK_ALG_NULL:
		if (level != SHRINK_L_NONE)
			goto fail;

		ctx->s_algorithm = "null";
		ctx->s_compress = s_compress_null;
		ctx->s_decompress = s_decompress_null;
		ctx->s_compress_bounds = s_compress_bounds_null;
		ctx->s_level = level;
		break;
#if defined(SUPPORT_LZO2)
	case SHRINK_ALG_LZO:
		if (lzo_init() != LZO_E_OK)
			return (NULL);
		switch (level) {
		case SHRINK_L_MIN:
			ctx->s_lzo1x_compress = lzo1x_1_compress;
			ctx->s_lzo1x_heapsz = LZO1X_1_MEM_COMPRESS;
			ctx->s_algorithm = "lzo1x_1";
			break;
		case SHRINK_L_MID:
			ctx->s_lzo1x_compress = lzo1x_1_15_compress;
			ctx->s_lzo1x_heapsz = LZO1X_1_15_MEM_COMPRESS;
			ctx->s_algorithm = "lzo1x_1_15";
			break;
		case SHRINK_L_MAX:
			ctx->s_lzo1x_compress = lzo1x_999_compress;
			ctx->s_lzo1x_heapsz = LZO1X_999_MEM_COMPRESS;
			ctx->s_algorithm = "lzo1x_999";
			break;
		case SHRINK_L_NONE:
		default:
			goto fail;
		}
		ctx->s_compress = s_compress_lzo;
		ctx->s_decompress = s_decompress_lzo;
		ctx->s_level = level;
		ctx->s_compress_bounds = s_compress_bounds_lzo;
		break;
#endif /* SUPPORT_LZO2 */
#if defined(SUPPORT_LZW)
	case SHRINK_ALG_LZW:
		switch (level) {
		case SHRINK_L_MIN:
			ctx->s_algorithm = "lzw_1";
			ctx->s_level = 1;
			break;
		case SHRINK_L_MID:
			ctx->s_algorithm = "lzw_6";
			ctx->s_level = 6; /* default */
			break;
		case SHRINK_L_MAX:
			ctx->s_algorithm = "lzw_9";
			ctx->s_level = 9;
			break;
		case SHRINK_L_NONE:
		default:
			goto fail;
		}
		ctx->s_compress = s_compress_lzw;
		ctx->s_decompress = s_decompress_lzw;
		ctx->s_compress_bounds = s_compress_bounds_lzw;
		break;
#endif /* SUPPORT_LZW */
#if defined(SUPPORT_LZMA)
	case SHRINK_ALG_LZMA:
		switch (level) {
		case SHRINK_L_MIN:
			ctx->s_algorithm = "lzma_0";
			ctx->s_level = 0;
			break;
		case SHRINK_L_MID:
			ctx->s_algorithm = "lzma_6";
			ctx->s_level = 5;
			break;
		case SHRINK_L_MAX:
			ctx->s_algorithm = "lzma_9";
			ctx->s_level = 9;
			break;
		case SHRINK_L_NONE:
		default:
			goto fail;
		}
		ctx->s_compress = s_compress_lzma;
		ctx->s_decompress = s_decompress_lzma;
		ctx->s_compress_bounds = s_compress_bounds_lzma;
		ctx->s_level = level;
		break;
#endif /* SUPPORT_LZMA */
	default:
		goto fail;
	}

	return (ctx);
fail:
	free(ctx);
	return (NULL);
}

void
shrink_cleanup(struct shrink_ctx *ctx)
{
	/* XXX cleanup library state if any? */
	if (ctx != NULL)
		free(ctx);
}

int
shrink_compress(struct shrink_ctx *ctx, uint8_t *src, uint8_t *dst, size_t len,
    size_t *comp_sz, struct timeval *elapsed)
{
	struct timeval		end, start;
	int			ret;

	/* sanity */
	if (ctx == NULL)
		return (SHRINK_INVALID);
	if (comp_sz == NULL)
		return (SHRINK_INTEGRITY);
	/* XXX some checks were compress_bounds, some were comp_sz < len */
	if (shrink_compress_bounds(ctx, *comp_sz) < len)

#ifdef HMMMMM
	if (*comp_sz < len)
#endif
		return (SHRINK_INTEGRITY);

	if (elapsed && gettimeofday(&start, NULL) == -1)
		return (SHRINK_LIBC);

	ret = ctx->s_compress(ctx, src, dst, len, comp_sz);

	if (elapsed) {
		if (gettimeofday(&end, NULL) == -1)
			return (SHRINK_LIBC);
		timersub(&end, &start, elapsed);
	}

	return (ret);
}
int
shrink_decompress(struct shrink_ctx *ctx, uint8_t *src, uint8_t *dst,
    size_t len, size_t *uncomp_sz, struct timeval *elapsed)
{
	struct timeval		end, start;
	int			ret;

	if (uncomp_sz == NULL)
		return (SHRINK_INTEGRITY);
	/* allow for incompressible margin */
	if (shrink_compress_bounds(ctx, *uncomp_sz) < len)
		return (SHRINK_INTEGRITY);

	if (elapsed && gettimeofday(&start, NULL) == -1)
		return (SHRINK_LIBC);

	ret = ctx->s_decompress(ctx, src, dst, len, uncomp_sz);

	if (elapsed) {
		if (gettimeofday(&end, NULL) == -1)
			return (SHRINK_LIBC);
		timersub(&end, &start, elapsed);
	}

	return (ret);
}

void *
shrink_malloc(struct shrink_ctx *ctx, size_t *sz)
{
	void		*p;
	size_t		 real_sz;

	if (ctx == NULL)
		return (NULL);
	if (sz == NULL)
		return (NULL);
	real_sz = shrink_compress_bounds(ctx, *sz);
	p = malloc(real_sz);
	if (p == NULL)
		return (NULL);

	*sz = real_sz;
	return (p);
}

size_t
shrink_compress_bounds(struct shrink_ctx *ctx, size_t sz)
{
	return (ctx->s_compress_bounds(ctx, sz));
}

const char *
shrink_get_algorithm(struct shrink_ctx *ctx)
{
	if (ctx == NULL)
		return (NULL);
	return (ctx->s_algorithm);
}

/* XXX old api kept for old software. not threadsafe in the slightest. */
static struct shrink_ctx *internal_ctx = NULL;

int
s_init(int algorithm, int level)
{
	internal_ctx = shrink_init(algorithm, level);

	return (internal_ctx ? SHRINK_OK : SHRINK_INVALID);

}

int
s_compress(uint8_t *src, uint8_t *dst, size_t len, size_t *comp_sz,
    struct timeval *elapsed)
{
	return (shrink_compress(internal_ctx, src, dst, len, comp_sz, elapsed));
}

int
s_decompress(uint8_t *src, uint8_t *dst, size_t len, size_t *uncomp_sz,
    struct timeval *elapsed)
{
	return (shrink_decompress(internal_ctx, src, dst, len, uncomp_sz,
	    elapsed));
}

void *
s_malloc(size_t *sz)
{
	return (shrink_malloc(internal_ctx, sz));
}

size_t
s_compress_bounds(size_t sz)
{
	return (shrink_compress_bounds(internal_ctx, sz));
}

const char *
s_get_algorithm(void)
{
	return (shrink_get_algorithm(internal_ctx));
}
