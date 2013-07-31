#include <time.h>

#if defined(__FreeBSD__)
#	include <sys/timespec.h>
#endif

#include <sys/types.h>
#include <timeconv.h>
#include <sys/sysctl.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/queue.h>

/*

	Next Gen functions. All functions and variables are prefixed with V_, so there are no more collisions.

*/


struct _string {
	char * text;
	int length;
};
typedef struct _string V_string;

/*
	Old functions
*/

struct _handle {
	V_string type;
	size_t size;
	void * data;
	void * private; /* Currently unused */
};

typedef struct _handle V_handle;

struct dial_search_pairs {
	char * key;
	char * value;
};

struct dialparse_v1 {
	V_string protocol;
	int size;
	union {
		struct {
			V_string host;
			V_string port;
		};
		V_string path;
		struct dial_search_pairs * key;
	};
};

struct errorcode_v1 {
     unsigned int numeric;
     V_string type;
     V_string message;
};

/*
       This is stub code to support a lock manager we don't have, but will eventually have to.

       Notes: domain is per-cluster, and refers to the namespace the path is relative to.
       Right now we only use domain = "hostfs", which is typically mapped differently on each node,
       to the 'local' filesystem.

*/

#define LOCK_NULL 0
#define LOCK_CONCURRENT_READ 1
#define LOCK_PROTECTED_READ 2
#define LOCK_CONCURRENT_WRITE 3
#define LOCK_PROTECTED_WRITE 4
#define LOCK_EXCLUSIVE 5


extern V_handle * acquire(const char * domain, const char * path, int type);
extern int release(V_handle * lockid);
extern time_t get_cluster_uptime(void);
extern V_handle * new_handle(size_t size, V_string type);
extern int error(V_handle * error);
extern void evoke_exit(V_handle * error);
extern void print_error(V_handle * error);

extern V_handle * dial(char *address, char *local);

extern V_handle * dialparse(char *address);

extern V_handle * announce(char * address);

extern int close_handle(V_handle * realhandle);
