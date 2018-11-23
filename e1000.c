#include "e1000.h"

// LAB 6: Your driver code here
struct e1000_tx_desc tx_desc_buf[TXRING_LEN] __attribute__ ((__aligned__(PGSIZE)));
struct e1000_data tx_data_buf[TXRING_LEN] __attribute__ ((__aligned__(PGSIZE)));

struct e1000_rx_desc rx_desc_buf[RXRING_LEN] __attribute__ ((__aligned__(PGSIZE)));
struct e1000_data rx_data_buf[RXRING_LEN] __attribute__ ((__aligned__(PGSIZE)));

static void
init_desc(){
  int i;
  for (i = 0; i < TXRING_LEN; ++i)
  {
    tx_desc_buf[i].buffer_addr = V2P(&tx_data_buf[i]);
    tx_desc_buf[i].upper.fields.status = E1000_TXD_STAT_DD;
  }

  for (i = 0; i < RXRING_LEN; ++i)
  {
    rx_desc_buf[i].buffer_addr = PADDR(&rx_data_buf[i]);
  }
}

static void
e1000_init(){
  assert(e1000[E1000_STATUS] == 0x80080783);
  e1000[E1000_TDBAL] = V2P(tx_desc_buf);
  e1000[E1000_TDBAH] = 0x0;
  e1000[E1000_TDH] = 0x0;
  e1000[E1000_TDT] = 0x0;
  e1000[E1000_TDLEN] = TXRING_LEN * sizeof(struct e1000_tx_desc);
  e1000[E1000_TCTL] = VALUEATMASK(1, E1000_TCTL_EN) |
            VALUEATMASK(1, E1000_TCTL_PSP) |
            VALUEATMASK(0x10, E1000_TCTL_CT) |
            VALUEATMASK(0x40, E1000_TCTL_COLD);
  e1000[E1000_TIPG] = VALUEATMASK(10, E1000_TIPG_IPGT) |
            VALUEATMASK(8, E1000_TIPG_IPGR1) |
            VALUEATMASK(6, E1000_TIPG_IPGR2);
  e1000[E1000_RAL] = 0x12005452;
  e1000[E1000_RAH] = 0x00005634 | E1000_RAH_AV;
  e1000[E1000_RDBAL] = PADDR(rx_desc_buf);
  e1000[E1000_RDBAH] = 0x0;
  e1000[E1000_RDLEN] = RXRING_LEN * sizeof(struct e1000_rx_desc);
  e1000[E1000_RDH] = 0x0;
  e1000[E1000_RDT] = RXRING_LEN;
  e1000[E1000_RCTL] = E1000_RCTL_EN |
            !E1000_RCTL_LPE |
            E1000_RCTL_LBM_NO |
            E1000_RCTL_RDMTS_HALF |
            E1000_RCTL_MO_0 |
            E1000_RCTL_BAM |
            E1000_RCTL_BSEX |
            E1000_RCTL_SZ_4096 |
            E1000_RCTL_SECRC;
}

int
e1000_attach(struct pci_func *pcif)
{
	/* Fill this in */
	pci_func_enable(pcif);
	// init descriptor
	init_desc();
	// create virtual memory mapping
	e1000 = mmio_map_region(pcif->reg_base[0], pcif->reg_size[0]);
	assert(e1000[E1000_STATUS] == 0x80080783);
	// init the hardware
  e1000_init();
	return 0;
}

int
e1000_transmit(const char *buf, unsigned int len)
{
	/* Fill this in */
	void* addr = (void*) buf;
	uint32_t tail = e1000[E1000_TDT];
  struct e1000_tx_desc * tail_desc = &tx_desc_buf[tail];
  if (tail_desc->upper.fields.status != E1000_TXD_STAT_DD)
  {
    return -1;
  }
  len = len > DATA_SIZE ? DATA_SIZE : len;
  memmove(&tx_data_buf[tail], addr, len);
  tail_desc->lower.flags.length = len;
  tail_desc->upper.fields.status = 0;
  tail_desc->lower.data |=  (E1000_TXD_CMD_RS |
                 E1000_TXD_CMD_EOP);
  e1000[E1000_TDT] = (tail + 1) % TXRING_LEN;
  return 0;
}

int
e1000_receive(char *buf, unsigned int len)
{
	/* Fill this in */
	return -1;
}
