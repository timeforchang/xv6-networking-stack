/**
 *author: Andrew Chang
 *
 *system call to manage your filter table
 *expects a char* IP address and direction of flow
 */

#include "types.h"
#include "defs.h"
#include "nic.h"

int sys_filter(void) {
  char *macAddr, *dir, *interface;
  int opt;

  if(argstr(0, &interface) < 0 || argsint(1, &opt) < 0 || argstr(2, &macAddr) < 0 || argstr(3, &dir) < 0) {
    cprintf("ERROR: sys_filter: Failed to fetch arguments");
    return -1;
  }

  struct nic_device *nd;
  if(get_device(interface, &nd) < 0) {
    cprintf("ERROR:sys_filter:Device not loaded\n");
    return -1;
  }

  if (opt == 0) {
    cprintf("removing filter MAC: %s going/coming %s\n", macAddr, dir);
  } else {
    cprintf("filtering MAC: %s going/coming %s\n", macAddr, dir);
  }
  
  nd->filter_packet(nd->driver, macAddr, dir, opt);

  return 0;
}
