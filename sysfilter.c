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
  safestrcpy(newEntry.dir, dir, 4);
  safestrcpy(newEntry.mac, macAddr, 17);
  cprintf("new entry: %s %s\n", newEntry.mac, newEntry.dir);
  for (int i = 0; i < ENTRYLIMIT; i++) {
    cprintf("still ok\n");
    // cprintf("%d\n", ebtable->init);
    //release(&ebtable->lock);
    //
    if (!ebtable->entries[i]) {
      cprintf("still ok2\n");
      memmove(ebtable->entries[i], (void*)&newEntry, sizeof(struct filter_entry));
    }
    //release(&ebtable->lock);
    //acquire(&ebtable->lock);
    cprintf("ebtable[%d]: %s %s\n", i, ebtable->entries[i]->dir, ebtable->entries[i]->mac);
  }

  return 0;
}
