#include "types.h"
#include "param.h"
#include "select.h"
#include "spinlock.h"
#include "defs.h"

extern void wakeup(void *);

/** Initializes a queue.
 *
 * @param {struct selproc *} s - the queue to be initialized
 */
void initselproc(struct selproc * s)
{
  s->selcount = 0;
  for (int i=0; i<NSELPROC; i++)
  {
      // set everything to 0 at first
      s->sel[i] = 0;
  }
}

/** Clears a wakeup call.
 *
 * @param {struct selproc *} s - the queue to be removed to
 * @param {int *} selid - the id to find the ip in the queue to remove it
 */
void clearselid(struct selproc *s, int * selid)
{
    for (int i=0; i<NSELPROC; i++)
    {
        if (s->sel[i] == selid)
        {
            // once you've found the ip to remove, clear it from the queue and decremene the selcount
            s->sel[i] = 0;
            s->selcount--;
        }
    }
}

/** Sets a wakeup call.
 *
 * @param {struct selproc *} s - the queue to be added to
 * @param {int *} selid - the id to find the ip in the queue
 * @param {struct spinlock *} lk - the lock that will sleep the ip until it wakes up
 */
void addselid(struct selproc *s, int * selid, struct spinlock * lk)
{
    for (int i=0; i<NSELPROC; i++)
    {
        if (s->sel[i] == 0)
        {
            s->sel[i] = selid;
            s->lk[i] = lk;
            s->selcount++;
            break;
        }
    }
}

/** Wakes up any waiting ips.
 *
 * @param {struct selproc *} s - the queue to search for an ip to wake up
 */
void wakeupselect(struct selproc * s)
{
    for (int i=0; i<NSELPROC; i++)
    {
        if (s->sel[i])
        {
            // first get the wakeup channel
            void *wakeup_chan = s->sel[i];
            s->sel[i] = 0; // clear in queue
            s->selcount--; // decrement selcount
            acquire(s->lk[i]);
            wakeup(wakeup_chan); // wakeup the channel
            release(s->lk[i]);
        }
    }
}
