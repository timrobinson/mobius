/* $Id: test.c,v 1.6 2002/01/05 00:54:11 pavlovskii Exp $ */

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include <os/syscall.h>
#include <os/port.h>
#include <os/defs.h>

process_info_t *ProcGetProcessInfo(void);

int main(void)
{
	/*uint32_t key;
	process_info_t *info;*/
	handle_t file;
	wchar_t name[] = L"/hd/test.txt";
	char key[4];
	wchar_t str[5];
	size_t len;
	
	wprintf(L"Hello from tty0!\n");
	wprintf(L"Here's an escape sequence: \x1b[31mThis should be red!\x1b[37m\n");
	wprintf(L"And this is \x1b[1;5;32mbright green and flashing!\n\x1b[37m");
	wprintf(L"This is the first of two lines of text.\x1b[0m\n"
		L"\x1b[1B\x1b[2CAnd this is the second...\n");
	wprintf(L"Now the third...\n"
		L"...and fourth lines\n");
	wprintf(L"Here's a tab, just for a laugh...\tHa ha!\n");
	
	file = FsOpen(name, FILE_READ);
	if (file == NULL)
		wprintf(L"Failed to open %s\n", name);
	else
	{
		while (FsRead(file, key, sizeof(key)))
		{
			/*wprintf(L"Read %u bytes; the device says: %02x %02x %02x %02x\n", 
				bytes, buf[0], buf[1], buf[2], buf[3]);*/
			len = mbstowcs(str, key, _countof(key));
			wprintf(L"%.*s", len, str);
		}

		FsClose(file);
	}

	/*info = ProcGetProcessInfo();
	while (FsRead(info->std_in, &key, sizeof(key)) == sizeof(key))
		wprintf(L"Key: %u\n", key);*/
	
	wprintf(L"Bye now...\n");
	return EXIT_SUCCESS;
}