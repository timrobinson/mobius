#include <wchar.h>
#include <errno.h>

/* xxx -- need to account for UTF-16 in these routines */

size_t wcstombs(char *mbstr, const wchar_t *wcstr, size_t count)
{
	int written, bytes;
	uint16_t w;

	for (written = 0; *wcstr && written < count;)
	{
		w = *wcstr;
		
		if (w <= 0x7f)
			bytes = 1;
		else if (w <= 0x7ff)
			bytes = 2;
		else if (w <= 0xffff)
			bytes = 3;
		else
		{
			errno = EILSEQ;
			return -1;
		}

		if (written + bytes > count)
			return written;

		switch (bytes)
		{
		case 1:
			mbstr[0] = w & 0x7f;
			break;

		case 2:
			mbstr[1] = 0x80 | (w & 0x3f);
			w >>= 6;
			mbstr[0] = 0xC0 | (w & 0x1f);
			break;

		case 3:
			mbstr[2] = 0x80 | (w & 0x3f);
			w >>= 6;
			mbstr[1] = 0x80 | (w & 0x3f);
			w >>= 6;
			mbstr[0] = 0xE0 | (w & 0x0f);
			break;
		}
		
		written += bytes;
		mbstr += bytes;
		wcstr++;
	}

	*mbstr = 0;
	return written;
}

