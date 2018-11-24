#ifndef __XV6_NETSTACK_FILTER_H__
#define __XV6_NETSTACK_FILTER_H__

#include "spinlock.h"
#define ENTRYLIMIT 32

struct filter_entry {
  char dir[4];
  char mac[6];
};

struct filter_table {
	int init;
	struct spinlock lock;
	struct filter_entry *entries[ENTRYLIMIT];
};

static struct filter_table *ebtable;

#endif