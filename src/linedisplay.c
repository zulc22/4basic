#include "linedisplay.h"
char* linedisplay_lines[LINEDISPLAY_MAX_LINES];
int linedisplay_current_line;

#include <stdlib.h>
#include <string.h>

void linedisplay_init()
{
	for (unsigned i = 0; i < LINEDISPLAY_MAX_LINES; i++) {
		linedisplay_lines[i] = NULL;
		ev_linedisplay_lines_updated(i);
	}
	linedisplay_current_line = 0;
}

#pragma warning( push )
// a warning comes from the dereferencing of malloc, which can possibly
// return a null pointer if it can't allocate the requested amount of memory.
// 
// i am going to intentionally ignore this because if we're failing to
// allocate 60 bytes of memory i want the program to crash anyway.
#pragma warning( disable : 6011 )
char* linedisplay_line_get(unsigned line_index)
{
	// allocate a line if it's empty
	if (linedisplay_lines[line_index] == NULL) {
		linedisplay_lines[line_index] =
			malloc(LINEDISPLAY_MAX_CHARS);
		linedisplay_lines[line_index][0] = 0;
	}
	return linedisplay_lines[line_index];
}
#pragma warning( pop )

char* linedisplay_line_next()
{
	if (linedisplay_current_line == LINEDISPLAY_MAX_LINES - 1) {
		linedisplay_scroll();
	} else linedisplay_current_line++;
	return linedisplay_line_get(linedisplay_current_line);
}

void linedisplay_scroll()
{
	// get a copy of the first line (to recycle anything already allocated)
	char* first_line = linedisplay_lines[0];

	// copy all the other lines up
	for (int i = 0; i < LINEDISPLAY_MAX_LINES-1; i++) {
		linedisplay_lines[i] = linedisplay_lines[i+1];
		ev_linedisplay_lines_updated(i);
	}

	// recycle the first line
	linedisplay_lines[LINEDISPLAY_MAX_LINES - 1] = first_line;
	// and clear it
	first_line[0] = 0;
	ev_linedisplay_lines_updated(LINEDISPLAY_MAX_LINES - 1);
}

void linedisplay_clear(unsigned line_index) {
	if (linedisplay_lines[line_index] != NULL)
		free(linedisplay_lines[line_index]);

	linedisplay_lines[line_index] = NULL;
	ev_linedisplay_lines_updated(line_index);
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
		if (*source == 0) {
			destination[offset] = 0;
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
	char* line = linedisplay_line_get(linedisplay_current_line);
	unsigned count = 0;
	for (; line[count] != 0; count++) {}
	linedisplay_safecopy(source_text, count);
}

#include <stdio.h>
#if !defined(GBA)
#define iprintf printf
#endif

//#include <Windows.h>
void ev_linedisplay_lines_updated(unsigned line_index)
{
	char* line = linedisplay_lines[line_index];
	/* VT100...   Print the line's text
	         Clear it -----.  |
	Goto the changed line  |  |
	            |          |  |
					 [---------][----][]*/
	iprintf("\033[%i;%iH\033[K%s",
		line_index + 1, 1, line==NULL?"" : line);
	//if (line!=NULL) Sleep(100);
}
