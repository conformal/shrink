#include <sys/types.h>
#include <stdlib.h>
void
arc4random_buf(void *buf, size_t nbytes)
{
	uint32_t		data;
	uint8_t			data8;
	uint8_t			*buf8 = buf;

	while (nbytes > 0) {
		data = arc4random();
		switch(nbytes) {
		default:
			/* fallthru */
		case 4:
			data8 = data & 0xff;
			data = data >> 8;
			*buf8++ = data8;
			/* fallthru */
		case 3:
			data8 = data & 0xff;
			data = data >> 8;
			*buf8++ = data8;
			/* fallthru */
		case 2:
			data8 = data & 0xff;
			data = data >> 8;
			*buf8++ = data8;
			/* fallthru */
		case 1:
			data8 = data & 0xff;
			*buf8++ = data8;
			break;
		}
		nbytes -= 4;
	}
}
