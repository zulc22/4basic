#include <stdio.h>
#include "input.h"
#include "linedisplay.h"

int main() {
	input_init();
	linedisplay_init();
	for (unsigned i = 0; i < LINEDISPLAY_MAX_LINES + 1; i++) {
		/*if (i == 18) {
			input_get_char();
		}*/
		char balls[LINEDISPLAY_MAX_CHARS];
		sprintf(&balls, "what up dude %u", i);
		linedisplay_append(balls);
		linedisplay_line_next();
	}
	printf("\033[%i;%iH? ", LINEDISPLAY_MAX_LINES + 2, 1);

	char buf[LINEDISPLAY_MAX_CHARS];
	input_get_line(&buf);
	linedisplay_println(&buf);

	printf("\033[%i;%iH",LINEDISPLAY_MAX_LINES+2,1);
	return 0;
}
