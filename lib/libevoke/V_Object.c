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

V_Object * V_ObjectNew(V_String type) {
	size_t size = sizeof(V_Object) + type.length;
	V_Object * pointer = malloc(size);
	pointer->type.text = (char *) pointer + sizeof(V_Object);
	pointer->type.length = type.length;
	bcopy(type.text, pointer->type.text, type.length);
	printf("V_Object: 0x%x\n", (size_t) pointer);
	printf("V_String: 0x%x \"%s\"\n", (size_t) pointer + (size_t) sizeof(V_Object), pointer->type.text);
	return pointer;
};

V_ObjectData * V_ObjectAddData(V_Object * object, V_String name, V_String type, void * data, size_t size) {
	printf("V_ObjectData: (%d)\n", (size_t) sizeof(V_ObjectData));
	printf("V_ObjectData Name: (%d) \"%s\"\n", (size_t) name.length, name.text);
	printf("V_ObjectData Type: (%d) \"%s\"\n", (size_t) type.length, type.text);
	printf("V_ObjectData Payload: (%d)\n", (size_t) size);
	printf("V_ObjectData Total: (%d)\n", (size_t) size + (size_t) sizeof(V_ObjectData) + (size_t) name.length + (size_t) type.length);
	return NULL;
};

V_ObjectData * V_ObjectFindData(V_Object * object, V_String name) {
	return NULL;
};
