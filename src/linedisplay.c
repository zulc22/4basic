#include "linedisplay.h"
char* linedisplay_lines[LINEDISPLAY_MAX_LINES];
int linedisplay_current_line;

#include <stdlib.h>
#include <string.h>

void linedisplay_init() {
	for (unsigned i = 0; i < LINEDISPLAY_MAX_LINES; i++) {
		linedisplay_lines[i] = NULL;
		ev_linedisplay_lines_updated(i);
	}
	linedisplay_current_line = 0;
}

char* linedisplay_line_get(unsigned line_index)
{
	if (linedisplay_lines[line_index] == NULL) {
		linedisplay_lines[line_index] =
			malloc(LINEDISPLAY_MAX_CHARS);
	}
	return linedisplay_lines[line_index];
}

char* linedisplay_line_next()
{
	if (linedisplay_current_line+1 == LINEDISPLAY_MAX_LINES - 1) {
		linedisplay_scroll();
	}
	linedisplay_current_line++;
	char* line = linedisplay_line_get(linedisplay_current_line);
	return line;
}

void linedisplay_scroll()
{
	// grab line 0
	char* line = linedisplay_line_get(0);
	// mark it as empty
	line[0] = 0;
	// copy all the other lines up
	for (int i = 0; i < linedisplay_current_line; i++) {
		linedisplay_lines[i] = linedisplay_lines[i+1];
		ev_linedisplay_lines_updated(i);
	}
	linedisplay_lines[linedisplay_current_line] = line;
	ev_linedisplay_lines_updated(linedisplay_current_line);
	linedisplay_current_line--;
}

void linedisplay_safecopy(const char* source, int offset)
{
	char* destination = linedisplay_line_get(linedisplay_current_line);
	while (1) {
		if (offset >= LINEDISPLAY_MAX_CHARS) {
			destination[offset] = 0;
			offset = 0;
			ev_linedisplay_lines_updated(linedisplay_current_line);
			destination = linedisplay_line_next();
		}
		destination[offset++] = *source++;
		if (*source-1 == 0) {
			break;
		}
	}
	ev_linedisplay_lines_updated(linedisplay_current_line);
}

void linedisplay_println(const char* source_line)
{
	linedisplay_safecopy(source_line, 0);
	linedisplay_line_next();
}

void linedisplay_append(const char* source_text)
{
	char* line = linedisplay_lines[linedisplay_current_line];
	unsigned count = 0;
	for (; line[count] != 0; count++) {}
	linedisplay_safecopy(source_text, count);
}

#include <stdio.h>
#if !defined(GBA)
#define iprintf printf
#endif

void ev_linedisplay_lines_updated(unsigned line_index)
{
	char* line = linedisplay_lines[line_index];
	if (line == NULL) line = "";
	/* VT100...   Print the line's text
	         Clear it -----.  |
	Goto the changed line  |  |
	            |          |  |
					 [---------][----][]*/
	iprintf("\033[%i;%iH\033[K%02i: %s",
		line_index+1, 1, line_index, line);
}
