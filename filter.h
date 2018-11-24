#ifndef __XV6_NETSTACK_FILTER_H__
#define __XV6_NETSTACK_FILTER_H__

#define ENTRYLIMIT 32

struct filter_entry {
  char dir[4];
  uint8_t dmac[6];
};

struct filter_table {
	struct filter_entry *entries[ENTRYLIMIT];
};

#endif