#include "V.h"


/*
	Routine that 'dials' a remote host, and returns a list of file descriptors.
	Similar in function to the plan 9 dial() function.
	address and local take the following form:

	Protocols:
		tcp - tcp protocol - form: tcp!host!port
		udp - udp protocol - form: udp!host!port
		sctp - sctp protocol - form: sctp!host!port
		unix - unix domain socket - form: unix!/absolute/path or unix!relative/path
*/
handle * dial(char * address, char * local) {

}
