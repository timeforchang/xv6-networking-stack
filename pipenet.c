#include "types.h"
#include "user.h"
#include "stat.h"
#include "syscall.h"
#include "select.h"

char * shargv[] = { "sh", 0 };

/** Function that launches a pipenet
 *
 * @see FD_ZERO()
 * @see FD_SET()
 * @see FD_ISSET()
 * @see sys_select()
 * @see sys_pipe()
 * @see sys_dup()
 * @see sys_fork()
 * @see sys_close()
 * @see sys_exec()
 * @see sys_write()
 * @see sys_read()
 */
void
pipenet(void)
{
    int toshfds[2], fromshfds[2];
    char buf[100];
    int bytesread;
    int pid;

    //  make pipes to read and write from
    if (pipe(toshfds) != 0)
    {
        printf(2, "Pipe failed!");
        exit();
    }
    if (pipe(fromshfds))
    {
        printf(2, "Pipe failed!");
        exit();
    }

    fd_set readfds, writefds; // make sets
    int nfds = 7; // we have 7 fds: 0, 1, 2 for stdio, and 4 other ones created by the pipes
    if ((pid = fork()) == 0) // if child
    {
        close(toshfds[1]);   // close write end
        close(fromshfds[0]); // close read end
        close(0);            // close console stdin
        dup(toshfds[0]);     // dup pipe to stdin
        close(1);            // close console stdout
        close(2);            // close console stderr
        dup(fromshfds[1]);   // dup pipe to stdout
        dup(fromshfds[1]);   // dup pipe to stderr
        close(toshfds[0]);   // close duplicated read fd
        close(fromshfds[1]); // close duplicated write fd
        exec(shargv[0], shargv); // invoke shell
    }
    else
    {
        close(toshfds[0]);  // close read end
        close(fromshfds[1]);// close write end
        printf(1,"\n\n ***** Starting PipeNet ***** \n\n");
        // keep looping because this is a terminal basically
        for (;;) {
          // set up all the fd sets every loop
          FD_ZERO(&readfds);
          FD_ZERO(&writefds);
          FD_SET(0,&readfds);
          FD_SET(1,&writefds);
          FD_SET(2,&writefds);
          FD_SET(fromshfds[0],&readfds);
          FD_SET(toshfds[1],&writefds);
          // select to read/write
          if (select(nfds, &readfds, &writefds) == 0) {
            for (int fd=0; fd<nfds; fd++)
            {
              if (FD_ISSET(fd,&readfds))
              {
                  if (fd == 0) {
                    printf(1, "\nReading from console\n");
                    bytesread = read(0, buf, sizeof(buf)); // read from console
                    write(toshfds[1], buf, bytesread); // write console data to pipe since pipe is always going to be writable
                    
                  } else {
                    printf(1, "\nReading from pipe\n");
                    bytesread = read(fromshfds[0], buf, sizeof(buf)); // read from pipe
                    if (write(1, buf, bytesread) < 0) // write data from pipe to console since console is always writable
                    {
                        printf(2, "Write error!");
                        exit();
                    }
                  }
              }
            }
          }
        }
      wait();
    }
}

/** Main method to run the pipenet
 *
 * @param argc number of strings pointed to by argv
 * @param argv argument vector to be passed into the function
 * @see pipenet()
 * The method will always call exit() at the end or else it will fall off the end of the routine.
 * Method signature conforms to how the main method signature is supposed to be written
 * The method itself does not use any of the arguments passed to it.
 */
int
main(void)
{
    pipenet();
    exit();
}
