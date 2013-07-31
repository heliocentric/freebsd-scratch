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

#include <evoke.h>
#include <sys/queue.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/uio.h>
#include <unistd.h>

struct host {
	LIST_ENTRY(host) hosts;
	int connect_mode;
	time_t boottime;
	handle * fdlist;
	string networkaddress;
	string hostname;
};
/*
char signature[4] = { 0xC9, 0xBB, 0xC8, 0xBC };
*/
char signature[4] = { 'w', 'H', 'a', 'T' };

LIST_HEAD(hostlist, host) mainlist = LIST_HEAD_INITIALIZER(mainlist);
struct hostlist *headp;

int find_nodes(int searchmode, char *host, char *hostname);

int connect_to_host(struct host *current_host, char *localaddress);

handle * parse_message(int fd);
handle * send_message(int fd, handle * message);

struct message {
	long opcode;
	char * message;
};

/*
	Host connect modes.
*/

#define direct 1
#define stun 2
#define ssh 3

/*
	Search modes.
*/

#define direct 1
#define global 2
#define google 3

int main(int argc, char *argv[]) {

	V_String typecode;

	typecode.text = "com.googlecode.evoke.test";
	typecode.length = strlen(typecode.text) + 1;

	V_Object * pointer;
	pointer = V_ObjectNew(typecode);

	handle * scratch;

	typecode.text = "text/plain";
	typecode.length = strlen(typecode.text) + 1;

	V_String name;
	name.text = "TestValue";
	name.length = strlen(name.text) + 1;

	void * testpointer = malloc(2);
	scratch = V_ObjectAddData(pointer, name, typecode, testpointer, 2);

	struct message * rmsg;
	if (argc == 4) {
		handle * server	= announce(argv[1]);

		if (error(server)) {
			print_error(server);
			evoke_exit(server);
		}

		LIST_INIT(&mainlist);
		find_nodes(direct, argv[2], argv[3]);

		struct host *current_host, *temp;
		int count = 1;
		LIST_FOREACH_SAFE(current_host, &mainlist, hosts, temp) {
			if (count <= 20) {
				if (connect_to_host(current_host, argv[1])) {
					printf("blah\n");
					int * fdlist = current_host->fdlist->data;
					printf("blah\n");
					scratch = parse_message(fdlist[0]);
					if (error(scratch)) {
					} else {
						rmsg = scratch->data;
					}
					++count;
				}
			} else {
				break;
			}
		}
		system("netstat -aln");
		return 0;
	} else {
		printf("netd needs three options:\n");
		printf("\t1:\tlisten port\n");
		printf("\t2:\tconnect host\n");
		printf("\t3:\thostname\n");
		return 23;
	}
}

int find_nodes(int searchmode, char *address, char *hostname) {
	struct host *only_host;

	only_host = malloc(sizeof(struct host));

	only_host->networkaddress.length = strlen(address);
	only_host->networkaddress.text = address;
	only_host->hostname.length = strlen(hostname);
	only_host->hostname.text = hostname;
	only_host->fdlist = (handle *) NULL;
	only_host->connect_mode = searchmode;
	LIST_INSERT_HEAD(&mainlist, only_host, hosts);

	return 0;
}

int connect_to_host(struct host *current_host, char *localaddress) {

	handle * dialup = dial(current_host->networkaddress.text, "0");
	if (error(dialup)) {
		strerror(65);
		return 0;
	}

	current_host->fdlist = dialup;
	int * fdlist;
	fdlist = current_host->fdlist->data;
	printf("Node {\n");
	printf("\thostname\t = \t%s;\n", current_host->hostname.text);
	printf("\taddress\t\t = \t%s;\n", current_host->networkaddress.text);
	printf("\tmode\t\t = \t%d;\n", current_host->connect_mode);
	printf("\tfd\t\t = \t0x%u;\n", fdlist[0]);
	printf("}\n");
	return 2;
}

handle * send_message(int fd, handle * message) {
	
}

handle * parse_message(int fd) {
	ssize_t num;
	uint32_t size;
	char tempsignature[4];
	num = read(fd, &tempsignature, 4);
	handle * returnvalue;

	if (num < 4) {
		return NULL;
	}

	if (!strncpy(tempsignature, signature, 4)) {
		return NULL;
	}
	long opcode;
	num = read(fd, &opcode, 4);
	if (num != 4) {
		return NULL;
	}

	num = read(fd, &tempsignature, sizeof(size));
	short convsize = ntohl(size);
	printf("0x%lu\n", convsize);

	string netd_message_type;

	netd_message_type.text = "com.googlecode.evoke.netdmessage.v1.0";
	netd_message_type.length = strlen(netd_message_type.text) + 1;

	returnvalue = new_handle(convsize + sizeof(struct message), netd_message_type);

	if (error(returnvalue)) {
		return returnvalue;
	}

	struct message * packet = returnvalue->data;
	packet->opcode = ntohl(opcode);
	packet->message = returnvalue->data + sizeof(struct message);
	num = read(fd, packet->message, convsize);

	if (num < convsize) { 
		free(returnvalue);
		return NULL;
	}

	return returnvalue;
}
