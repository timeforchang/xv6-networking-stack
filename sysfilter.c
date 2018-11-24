/**
 *author: Andrew Chang
 *
 *system call to manage your filter table
 *expects a char* IP address and direction of flow
 */

#include "types.h"
#include "defs.h"
#include "filter.h"

int sys_filter(void) {
  char *macAddr, *dir;
  static struct filter_entry *entries[ENTRYLIMIT];

  if(argstr(0, &macAddr) < 0 || argstr(1, &dir) < 0) {
    cprintf("ERROR: sys_filter: Failed to fetch arguments");
    return -1;
  }

  cprintf("filtering MAC: %s going/coming %s\n", macAddr, dir);
  struct filter_entry newEntry = {dir, macAddr};
  for (int i = 0; i < ENTRYLIMIT; i++) {
    if (!entries[i]) {
      entries[i] = newEntry;
    } else {
      cprintf("entry exists at index %d\n", i);
    }
  }

  return 0;
}
