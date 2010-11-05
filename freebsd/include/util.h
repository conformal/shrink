#include <sys/types.h>
#include <conformal_lens.h>
#include <libutil.h>
/* this is not included in any standard place on freebsd, so provide one here */
#define      MIN(a,b) (((a)<(b))?(a):(b))
/* fmt_scaled  is not in util everywhere */
int fmt_scaled(long long number, char *result);
#define     FMT_SCALED_STRSIZE      7       /* minus sign, 4 digits, suffix, null byte */
