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

  if(argstr(0, &macAddr) < 0 || argstr(1, &dir) < 0) {
    cprintf("ERROR: sys_filter: Failed to fetch arguments");
    return -1;
  }

  cprintf("filtering MAC: %s going/coming %s\n", macAddr, dir);
  struct filter_entry newEntry;
  newEntry->dir = dir;
  newEntry->mac = macAddr;
  for (int i = 0; i < ENTRYLIMIT; i++) {
    if (!ebtable.entries[i]) {
      ebtable.entries[i] = newEntry;
    }
    cprintf("ebtable[%d]: %s %s\n", i, ebtable.entries[i]->dir, ebtable.entries[i]->mac);
  }

  return 0;
}
