#include "types.h"
#include "user.h"
#include "stat.h"
#include "syscall.h"
#include "select.h"

/** Test to see if setting the bits in fd sets actually works
 *
 * @see FD_ZERO()
 * @see FD_SET()
 * @see FD_ISSET()
 * @return 0 if nothing has gone wrong, 1 if there is some failure
 */
int
test0(void)
{
   fd_set s;
   int nfds = 32;
   FD_ZERO(&s);
   if (s != 0)
   {   
        printf(1, "FD_ZERO FAILED: s = %x\n", s);
        return 1;
   }
   for (int fd=0; fd<nfds; fd++)
   {
       FD_SET(fd, &s);
       if (!FD_ISSET(fd, &s))
       {
           printf(1, "FD_SET FAILED; s = %x\n", s);
           return 1;
       }
       FD_CLR(fd, &s);
       if (FD_ISSET(fd, &s))
       {
           printf(1, "FD_CLR FAILED: s = %x\n", s);
           return 1;
       }
   }
   
   // if you reach this point, nothing has gone wrong so print a success message
   printf(1, "TEST0 SUCCESS!\n");
   return 0;
}

/** Test to see if selecting on pipes works correctly
 *
 * @see sys_select()
 * @see FD_ZERO()
 * @see FD_SET()
 * @see FD_ISSET()
 * @return 0 if nothing has gone wrong, 1 if there is some failure
 */
int
test1(void)
{
    fd_set s;
    int nfds = 5;
    int fds[2];
    
    FD_ZERO(&s);

    pipe(fds);

    if (fork() == 0)
    {
        close(fds[0]);
        fd_set readfds, writefds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(fds[1],&writefds);
        if (select(nfds, &readfds, &writefds) == 0)
        {
            close(fds[1]);
            if (!FD_ISSET(fds[1],&writefds))
            {
                printf(1, "Child: select returned but write fd not set! \t FAIL\n");
                return 1;
            }
        }

        exit();

    } 
    else
    {
        close(fds[1]);
        fd_set readfds, writefds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        FD_SET(fds[0],&readfds);
        if (select(nfds, &readfds, &writefds) == 0)
        {
            close(fds[0]);
            if (!FD_ISSET(fds[0],&readfds))
            {
                printf(1, "Parent: select returned but read fd not set! \t FAIL\n");
                wait();
                return 1;
            } 
        }
        wait();
    }
    // if you reached this point, nothing has gone wrong so return 0 and print success message
    printf(1, "TEST1 SUCCESS!\n");
    return 0;
}

/** Test to see if selecting on console works correctly
 *
 * @see sys_select()
 * @see FD_ZERO()
 * @see FD_SET()
 * @see FD_ISSET()
 * @return 0 if nothing has gone wrong, 1 if there is some failure
 */
int
test2(void)
{
    fd_set s;
    int nfds = 3;
    
    FD_ZERO(&s);

    fd_set readfds, writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(0,&readfds);
    FD_SET(1,&writefds);
    FD_SET(2,&writefds);
    int correct = 0;
    // only fd 0 is readable, and should be set
    // fds 1 and 2 are only writeable, so they should only be set
    // to check, then you need all 6 cases to be correct
    if (select(nfds, &readfds, &writefds) == 0)
    {
        for (int fd=0; fd<nfds; fd++)
        {
            if (FD_ISSET(fd,&readfds))
            {
                printf(1, "Console read fd %d set\n", fd);
                if (fd == 0) {
                    correct++;
                }
            }
            else
            {
                printf(1, "Console read fd %d not set\n", fd);
                if (fd != 0) {
                    correct++;
                }

            }
            if (FD_ISSET(fd,&writefds))
            {
                printf(1, "Console write fd %d set\n", fd);
                if (fd != 0) {
                    correct++;
                }
            }
            else
            {
                printf(1, "Console write fd %d not set\n", fd);
                if (fd == 0) {
                    correct++;
                }
            }
        }
    }
    printf(1, "correct: %d\n", correct);
    // if the aformentioned 6 cases are all correct, print a success message
    if (correct == 6) {
        printf(1, "TEST2 SUCCESS!\n");
    }
    return 0;
}

/** Test to see if selecting on console works correctly when console hasn't been set to write prior
 *
 * @see sys_select()
 * @see FD_ZERO()
 * @see FD_SET()
 * @see FD_ISSET()
 * @return 0 if nothing has gone wrong, 1 if there is some failure
 */
int
test3(void)
{
    fd_set s;
    int nfds = 3;
    
    FD_ZERO(&s);

    fd_set readfds, writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_SET(0,&readfds);
    int correct = 0;
    // only need to check if fd 0 is set to readable
    // select sleeps until something is entered, so you won't go into the 
    // if statement until something is written into the console
    if (select(nfds, &readfds, &writefds) == 0)
    {
        for (int fd=0; fd<nfds; fd++)
        {
            if (FD_ISSET(fd,&readfds))
            {
                printf(1, "Console fd 0 set\n");
                if (fd == 0) {
                    correct = 1;
                }
            }
            else
            {
                printf(1, "Console fd 0 not set to read\n", fd);
            }
            if (FD_ISSET(fd,&writefds))
            {
                printf(1, "Console write fd %d set\n", fd);
            }
            else
            {
                printf(1, "Console write fd %d not set\n", fd);
            }

        }
    }
    // if 0 is still readable, print a success message
    if (correct) {
        printf(1, "TEST3 SUCCESS!\n");
    }
    return 0;
}

/** Test to see if selecting on pipe works correctly when pipe hasn't been set to write prior
 *
 * @see sys_select()
 * @see FD_ZERO()
 * @see FD_SET()
 * @see FD_ISSET()
 * @return 0 if nothing has gone wrong, 1 if there is some failure
 */
int
test3p(void)
{
    fd_set s;
    int nfds = 5;
    int fds[2];
    char wbuf[32] = "I am a write buffer\n";
    char rbuf[32];
    
    pipe(fds);
    
    FD_ZERO(&s);

    fd_set readfds, writefds;
    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    int correct = 0;
    if (fork() == 0)
    {
        FD_SET(fds[0],&readfds);
        printf(1, "Selecting on Read\n");
        if (select(nfds, &readfds, &writefds) == 0)
        {
            for (int fd=0; fd<nfds; fd++)
            {
                if (FD_ISSET(fd,&readfds))
                {
                    printf(1, "fd %d set read\n", fd);
                    // the fd created will only be fd 3 provided no other pipes are currently open
                    // it should also be readable by now since select has slept until something was
                    // written to it
                    if (fd == fds[0]) {
                        correct = 1;
                    }
                }
                else
                {
                    printf(1, "fd %d not set to read\n", fd);
                }
                if (FD_ISSET(fd,&writefds))
                {
                    printf(1, "fd %d set write\n", fd);
                }
                else
                {
                    printf(1, "fd %d not set to write\n", fd);
                }
            }
            read(fds[0],rbuf,32);
            write(1,rbuf,32);
        }
    }
    else
    {
        printf(1, "Sleeping for 100\n");
        sleep(100);
        printf(1, "Writing to pipe\n");
        write(fds[1],wbuf,32);
        wait();
    }
    // if fd 3 is now readable, print a success message
    if (correct) {
        printf(1, "TEST3P SUCCESS!\n");
    }
    return 0;
}

/** Main method to run all of the tests
 *
 * @param argc number of strings pointed to by argv
 * @param argv argument vector to be passed into the function
 * @see test0()
 * @see test1()
 * @see test2()
 * @see test3p()
 * @see test3()
 * The method will always call exit() at the end or else it will fall off the end of the routine.
 * Method signature conforms to how the main method signature is supposed to be written
 * The method itself does not use any of the arguments passed to it.
 */
int
main(void)
{

    printf(1, "test0\n");
    test0();

    printf(1, "\ntest1\n");
    test1();

    printf(1, "\ntest2\n");
    test2();

    printf(1, "\ntest3p\n");
    test3p();

    printf(1, "\ntest3\n");
    test3();
    exit();
}
