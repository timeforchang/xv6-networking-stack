#ifndef __XV6_NETSTACK_FILTER_H__
#define __XV6_NETSTACK_FILTER_H__

#define ENTRYLIMIT 32

struct filter_entry {
  char dir[4];
  uint8_t mac[6];
};

struct filter_table {
	int init;
	struct filter_entry *entries[ENTRYLIMIT];
};

struct filter_table *ebtable;

#endif