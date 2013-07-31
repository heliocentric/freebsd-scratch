#include "V.h"


/*
	Routine that 'dials' a remote host, and returns a list of file descriptors.
	Similar in function to the plan 9 dial() function.
	Main difference being we support returning multiple file descriptors.
	We also use a completely different format, taken from urls rather then plan9.
	This eases understanding.

	format:

		[transport[+mode]!]protocol:[//hostname][:port]
		[transport[+mode]!]protocol:[/path/to/file]
		[transport[+mode]!]protocol:[domain]
		[transport[+mode]!]protocol:

*/
handle * dial(char * address, char * local) {

}
