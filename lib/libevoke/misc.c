/*
# Copyright 2007-2010 Dylan Cochran
# All rights reserved
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted providing that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
# OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.

# $Id$

*/

#include <errno.h>
#include "evoke.h"

time_t get_cluster_uptime() {
	time_t uptime;
        struct timespec tp;
	char string[42];
	char *addrstring = string;
	char *newstring;
	int size = 42;
	if (sysctlbyname("kern.evoke_boottime", &string, &size, NULL, 0) != -1) {
		if (strncmp(string, "NULL", 5) != 0) {
			newstring = strsep(&addrstring, ",");
			if (newstring != NULL) {
				if (newstring != '\0') {
					long temptime = strtol(newstring, (char **)NULL , 0);
					time_t boottime = _long_to_time(temptime);
					uptime = time(NULL) - boottime;

				} else {
					if (clock_gettime(CLOCK_MONOTONIC, &tp) != -1) {
						uptime = tp.tv_sec;
					}
				}
			} else {
				if (clock_gettime(CLOCK_MONOTONIC, &tp) != -1) {
					uptime = tp.tv_sec;
				}
			}
		} else {
			if (clock_gettime(CLOCK_MONOTONIC, &tp) != -1) {
				uptime = tp.tv_sec;
			}
		}
	} else {
		if (clock_gettime(CLOCK_MONOTONIC, &tp) != -1) {
			uptime = tp.tv_sec;
		}
	}
	return uptime;
}

/* 
	function to create a handle with a data size as specified in 'size', fully initialized.
*/

handle * new_handle(size_t size, string type) {
	size_t allocsize;
	size_t handle_length = sizeof(handle);

	allocsize = handle_length + type.length + size;

	handle * temp = malloc(allocsize);


	temp->type.text = (char *) ((size_t) temp + handle_length);
	strncpy(temp->type.text, type.text, type.length);
	temp->type.length = type.length;
	temp->data = (void *) ((size_t) temp->type.text + type.length);
	temp->size = size;

/*
	printf("temp: %s\n", temp->type.text);
	printf("temp: %u\n", allocsize);
	printf("temp: %u\n", temp);
	printf("temp: %u\n", handle_length);
	printf("temp: %u\n", temp->type.text);
	printf("temp: %u\n", temp->type.length);
	printf("temp: %u\n", temp->data);
	printf("temp: %u\n", size);
	printf("temp: %u\n", (size_t) temp + (size_t) allocsize);
*/

	bzero(temp->data, size);

	return temp;
};

int close_handle(handle * handle) {
	free(handle);
	return 0;
}


/* 
	Function specification:

		address = a character string containing a plan9ish address specification.
			Currently the following formats are valid:

				net!host!port
				- 'Network' specification, ie, tcp/udp. You can specify tcp or udp, which will force it to use that protocol. 'net' is a basic specification, to find the 'best' protocol to use.
				- The host and port specification may be either symbolic or numeric.

				unix!/path/name
				- 'AF_UNIX' socket specification.

				nnet!key=value!key=value!key=value
				- key and value MUST NOT contain the characters '=', '!', or '\0'; all others are valid.
				- This method is the reason for MIMO semantics, because it is a 'search' rather then a specification. Multiple can be returned, and must be handled.

		local = Identical to address in format, but specifies the local address, local port, etc, when a socket is bound.
		fdlist = A 'handle' referring to the fdlist dial creates. In error, this handle remains NULL.
*/

int error(handle * error) {
	if (error == NULL) {
		return 2;
	} else {
		return 0;
	}

}

void print_error(handle * error) {
}

void evoke_exit(handle * error) {
	exit(1);
}

handle * dial(char *address, char *local) {
	handle * tempfd;
	char buffer[256];

	handle * dp;
	dp = dialparse(address);
	if (error(dp)) {
		printf("%s\n", strerror(65));
		return NULL;
	}
	struct dialparse_v1 * hostspec = dp->data;

	if (hostspec->protocol.length >= 4) {
		if (strncmp(hostspec->protocol.text,"net",4) == 0 || strncmp(hostspec->protocol.text,"tcp",4) == 0 || strncmp(hostspec->protocol.text,"udp",4) == 0 || strncmp(hostspec->protocol.text,"sctp",5) == 0) {
			string fdlist_type;
			fdlist_type.text = "com.googlecode.evoke.fdlist.v1.0";
			fdlist_type.length = strlen(fdlist_type.text) + 1;
			tempfd = new_handle(sizeof(int), fdlist_type);

			int * fdlist;
			int portnum;
			struct servent * realport;
			fdlist = tempfd->data;

			handle * dp2;

			dp2 = dialparse(local);

			if (! error(dp2)) {
				struct dialparse_v1 * localaddress = dp2->data;
				struct sockaddr_in bind_address;
				struct hostent *bind_host;
				if (localaddress->port.text == '\0') {
					portnum = 0;
				} else {
					realport = getservbyname(localaddress->port.text, "tcp");
					if(!realport) {
						portnum = strtonum(localaddress->port.text, 1, 65535, NULL);
						if (portnum == 0) {
							portnum = 0;
						}
					} else {
						portnum = ntohs(realport->s_port);
					}
				}
				bzero((char *) &bind_address, sizeof(bind_address));
				bind_address.sin_family = AF_INET;

				if (localaddress->host.length == 2) {
					if (strncmp(localaddress->host.text, "*", 2) == 0) {
						bind_address.sin_addr.s_addr = htonl(INADDR_ANY);
					} else {
						bind_host = gethostbyname(localaddress->host.text);
						bcopy((char *) bind_host->h_addr, (char *) &bind_address.sin_addr.s_addr, bind_host->h_length);
					}
				} else {
					bind_host = gethostbyname(localaddress->host.text);
					bcopy((char *) bind_host->h_addr, (char *) &bind_address.sin_addr.s_addr, bind_host->h_length);
				}

				bind_address.sin_port = htons(portnum);

				if (bind(fdlist[0], (struct sockaddr *) &bind_address, sizeof(bind_address)) < 0) {
					return NULL;
				}
			}

			struct addrinfo hints, *res, *res0;
			struct sockaddr_in targetaddress;
			int retval;
			const char *cause = NULL;
			hints.ai_family = PF_UNSPEC;

			if (strncmp(hostspec->protocol.text,"udp",4) == 0) {
				hints.ai_socktype = SOCK_DGRAM;
				hints.ai_protocol = IPPROTO_UDP;
			} else if (strncmp(hostspec->protocol.text,"sctp",5) == 0) {
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_SCTP;
			} else if (strncmp(hostspec->protocol.text,"tcp",5) == 0) {
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;
			} else {
				hints.ai_socktype = SOCK_STREAM;
			}

			retval = getaddrinfo(hostspec->host.text, hostspec->port.text, &hints, &res);
			if (retval) {
				return NULL;
			}

			fdlist[0] = -1;

			for (res = res0; res; res = res->ai_next) {
				fdlist[0] = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
				if (fdlist[0] < 0) { 
					cause = "socket";
					continue;
				}
				if (connect(fdlist[0], res->ai_addr, res->ai_addrlen) < 0) {
					cause = "connect";
					close(fdlist[0]);
					fdlist[0] = -1;
					continue;
				}
				break;
			}

			if (fdlist[0] < 0) {
				return NULL;
			}

			freeaddrinfo(res0);
		}

	}
	return tempfd;
}

handle * announce(char *address) {

#define		SOCK_MAX	128

	handle * ap = NULL;
	handle * tempfd = NULL;
	struct sockaddr_in bind_address;
	struct hostent *bind_host;


	ap = dialparse(address);

	if (error(ap)) {
		return NULL;
	}

	struct dialparse_v1 * localaddress = ap->data;

	if (localaddress->port.text == NULL) {
		return NULL;
	}


	string fdlist_type;
	fdlist_type.text = "com.googlecode.evoke.fdlist.v1.0";
	fdlist_type.length = strlen(fdlist_type.text) + 1;

	struct addrinfo hints, *res, *res0;
	int retval;
	int socklist[SOCK_MAX];
	int nsocklist[SOCK_MAX];
	int nsock;
	const char *cause = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_flags = AI_PASSIVE;


	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (localaddress->protocol.length >= 4) {
		if (strncmp(localaddress->protocol.text, "udp", 4) == 0) { 
			hints.ai_socktype = SOCK_DGRAM;
			hints.ai_protocol = IPPROTO_UDP;
		} else if (strncmp(localaddress->protocol.text, "tcp", 4) == 0) {
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;
		} else if (strncmp(localaddress->protocol.text, "sctp", 5) == 0) {
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_SCTP;
		} else if (strncmp(localaddress->protocol.text, "net", 4) == 0) {
			hints.ai_socktype = SOCK_STREAM;
		} else {
			return NULL;
		}
	} else {
		return NULL;
	}

	if (localaddress->host.length == 2) {
		if (strncmp(localaddress->host.text, "*", 2) == 0) {
			retval = getaddrinfo(NULL, localaddress->port.text, &hints, &res0);
		} else {
			retval = getaddrinfo(localaddress->host.text, localaddress->port.text, &hints, &res0);
		}
	} else {
		retval = getaddrinfo(localaddress->host.text, localaddress->port.text, &hints, &res0);
	}

	if (retval) {
		return NULL;
	}

	nsock = 0;
	for (res = res0; res && nsock < SOCK_MAX; res = res->ai_next) {
		socklist[nsock] = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socklist[nsock] < 0) {
			cause = "socket";
			continue;
		}
		if (bind(socklist[nsock], res->ai_addr, res->ai_addrlen) < 0) {
			cause = "bind";
			close(socklist[nsock]);
			continue;
		}
		(void) listen(socklist[nsock], 5);
		nsocklist[nsock] = socklist[nsock];
		nsock++;
	}

	if (nsock == 0) {
		return NULL;
	}
	
	freeaddrinfo(res0);
	tempfd = new_handle(sizeof(int) * nsock, fdlist_type);
	int * fdlist;
	fdlist = tempfd->data;
	bcopy(nsocklist, fdlist, sizeof(int) * nsock);
	return tempfd;
}

handle * dialparse(char *address) {
	handle * pointer;
	pointer = NULL;
	char *tempaddress, *realaddress;
	tempaddress = strdup(address);
	realaddress = tempaddress;
	char *protocol;
	protocol = strsep(&tempaddress, "!");

	if (protocol == '\0' || protocol == NULL) {
		printf("%s\n", strerror(22));
		return NULL;
	}

	size_t structsize = sizeof(struct dialparse_v1);

	printf("protocol = %s\n", protocol);

	size_t protocolsize = strlen(protocol) + 1;

	if (protocolsize >= 4) {
		if (strncmp("net", protocol,4) == 0 || strncmp("tcp", protocol, 4) == 0 || strncmp("udp",protocol,4) == 0 || strncmp("sctp",protocol,5) == 0) {
			char *host;
			host = strsep(&tempaddress, "!");

			char *port;
			port = strsep(&tempaddress, "!");

			if (host == '\0') {
				printf("%s\n", strerror(22));
				return NULL;
			}

			printf("host = %s\n", host);
			size_t portsize = 0;
			if (port != '\0') {
				portsize = strlen(port) + 1;
				printf("port = %s\n", port);
			}
			size_t hostsize = strlen(host) + 1;
			size_t totalsize = structsize + protocolsize + hostsize + portsize;
			string dialparse_type;
			dialparse_type.text = "com.googlecode.evoke.dialparse.v1.0";
			dialparse_type.length = strlen(dialparse_type.text) + 1;

			pointer = new_handle(totalsize, dialparse_type);
			struct dialparse_v1 * temp;
			temp = pointer->data;
	
			temp->protocol.text = (char *) ((size_t) temp + structsize);
			temp->protocol.length = protocolsize;
			strncpy(temp->protocol.text, protocol, protocolsize);

			temp->host.text = (char *) ((size_t) temp + structsize + protocolsize);
			temp->host.length = hostsize;
			strncpy(temp->host.text, host, hostsize);

			temp->port.text = NULL;
			temp->port.length = 0;
			if (port != '\0') {
				temp->port.text = (char *) ((size_t) temp + structsize + protocolsize + hostsize);
				temp->port.length = portsize;
				strncpy(temp->port.text, port, portsize);
			}
		}
	}
	free(realaddress);

	return pointer;
}
