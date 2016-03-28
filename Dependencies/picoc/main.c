/* picoc main program - this varies depending on your operating system and
 * how you're using picoc */

/* include only picoc.h here - should be able to use it with only the external interfaces, no internals from interpreter.h */
#include "picoc.h"

/* platform-dependent code for running programs is in this file */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PICOC_STACK_SIZE (128*1024)              /* space for the the stack */

int main(int argc, char **argv)
{
	Picoc pc;
	PicocInitialise(&pc, PICOC_STACK_SIZE);

	if (PicocPlatformSetExitPoint(&pc))
	{
		goto RET;
	}

	PicocIncludeAllSystemHeaders(&pc);
	PicocPlatformScanFile(&pc, "test.c");

	PicocCallEnable(&pc);
	PicocCallDisable(&pc);
	PicocCallUpdate(&pc, 0.1f);
	PicocCallFixedUpdate(&pc, 0.1f);

RET:
	PicocCleanup(&pc);
	return pc.PicocExitValue;
}
