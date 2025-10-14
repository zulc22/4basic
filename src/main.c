#include <stdio.h>
#include "input.h"
#include "linedisplay.h"

int main() {
	linedisplay_init();
	for (unsigned i=0; i<LINEDISPLAY_MAX_LINES-2; i++)
		linedisplay_println("what up dude");

	printf("\033[%i;%iH",LINEDISPLAY_MAX_LINES+2,1);
	return 0;
}
