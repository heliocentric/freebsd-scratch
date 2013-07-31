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

#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/sysctl.h>

char evoke_boottime[42] = "NULL";
SYSCTL_STRING(_kern, OID_AUTO, evoke_boottime, CTLFLAG_RW, evoke_boottime, sizeof(evoke_boottime), "Evoke specific boot time");

char evoke_version[256] = "%%VERSION%%";
SYSCTL_STRING(_kern, OID_AUTO, evoke_version, CTLFLAG_RW, evoke_version, sizeof(evoke_version), "Evoke system version");

char evoke_revision[256] = "%%REVISION%%";
SYSCTL_STRING(_kern, OID_AUTO, evoke_revision, CTLFLAG_RW, evoke_revision, sizeof(evoke_revision), "Evoke system revision");

static int load(struct module *module, int cmd, void *arg) {
	int error = 0;
	switch (cmd) {
		case MOD_LOAD:
		break;
		case MOD_UNLOAD:
		break;
		default:
			error = EOPNOTSUPP;
		break;
	}
	return(error);

}

static moduledata_t evoke_mod = {
	"evoke",
	load,
	NULL
};

DECLARE_MODULE(evoke, evoke_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);
