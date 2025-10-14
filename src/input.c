#include "input.h"
#include "linedisplay.h"

void input_get_line(char* out_line) {
	for (unsigned characters_written = 0;
		characters_written < LINEDISPLAY_MAX_CHARS;
		characters_written++) {
		out_line[characters_written] = input_get_char();
		if (out_line[characters_written] == '\n') {
			out_line[characters_written] = 0;
			return;
		}
	}
}

#if DEFINED(WIN32)

#include <windows.h>

HANDLE hConsoleInput;
DWORD dwRead;

void input_init() {
	hConsoleInput = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(hConsoleInput, 0);
}

char input_get_char() {
	char out;
	DWORD num_read = 0;
	while (num_read == 0)
		ReadConsole(hConsoleInput, &out, 1, &num_read, NULL);
	return out;
}

#else

#include <stdio.h>

void input_init() {
	// enable raw mode or something lole
	//printf("");
}

char input_get_char() {
	return (char)getchar();
}

#endif
