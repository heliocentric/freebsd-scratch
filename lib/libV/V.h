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


typedef struct _string V_String;

#define string V_String



typedef struct _V_Handle V_Handle;



struct _V_Object {
	V_String type;
	void * private;
	LIST_HEAD(_datahead, _V_ObjectData) datahead;
};

typedef struct _V_Object V_Object;

struct _V_ObjectData {
        LIST_ENTRY(_V_ObjectData) datalist;
	V_String name;
	size_t size;
	V_String type;
	void * data;
};

typedef struct _V_ObjectData V_ObjectData;


extern V_Object * V_ObjectNew(V_String type);
extern V_ObjectData * V_ObjectAddData(V_Object * object, V_String name, V_String type, void * data, size_t size);
extern V_ObjectData * V_ObjectFindData(V_Object * object, V_String name);


extern V_Object * V_ErrorObject(int number, V_String type, V_String message);
extern int V_Error(V_Object * ErrorObject);
extern int V_PrintError(V_Object * ErrorObject);
extern int V_Exit(V_Object * ErrorObject);

/* 
	Old functions
*/

struct _handle {
	string type;
	size_t size;
	void * data;
	void * private; /* Currently unused */
};

typedef struct _handle handle;


struct dial_search_pairs {
	char * key;
	char * value;
};

struct dialparse_v1 {
	string protocol;
	int size;
	union {
		struct {
			string host;
			string port;
		};
		string path;
		struct dial_search_pairs * key;
	};
};

struct errorcode_v1 {
     unsigned int numeric;
     string type;
     string message;
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


extern handle * acquire(const char * domain, const char * path, int type);
extern int release(handle * lockid);
extern time_t get_cluster_uptime(void);
extern handle * new_handle(size_t size, string type);
extern int error(handle * error);
extern void evoke_exit(handle * error);
extern void print_error(handle * error);

extern handle * dial(char *address, char *local);

extern handle * dialparse(char *address);

extern handle * announce(char * address);

extern int close_handle(handle * realhandle);
