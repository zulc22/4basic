#pragma once

#define LINEDISPLAY_MAX_LINES 20
#define LINEDISPLAY_MAX_CHARS 60

void linedisplay_init();

extern char* linedisplay_lines[LINEDISPLAY_MAX_LINES];
extern unsigned linedisplay_current_line;
//#if defined(GBA)
//extern bool linedisplay_horiz_scrolled;
//#endif

// Return a pointer to the specified line.
char* linedisplay_line_get(unsigned line_index);

// Return a pointer to a new line in the linedisplay buffer.
char* linedisplay_line_next();

// Scroll the buffer upwards (for when it is full.)
void linedisplay_scroll();

// Print a new line onto the line display.
void linedisplay_println(const char* source_line);

// Append onto the currently active line on the display.
void linedisplay_append(const char* source_text);

// Clear a line
void linedisplay_clear(unsigned line_index);

// Copy into the current line (internal)
void linedisplay_safecopy(const char* source, unsigned offset);

// Redraw a specific line at the output. (Run whenever a line is updated)
void ev_linedisplay_lines_updated(unsigned line_index);
