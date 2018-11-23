xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix
Version 6 (v6).  xv6 loosely follows the structure and style of v6,
but is implemented for a modern x86-based multiprocessor using ANSI C.

ACKNOWLEDGMENTS

xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)). See also http://pdos.csail.mit.edu/6.828/2016/xv6.html, which
provides pointers to on-line resources for v6.

xv6 borrows code from the following sources:
    JOS (asm.h, elf.h, mmu.h, bootasm.S, ide.c, console.c, and others)
    Plan 9 (entryother.S, mp.h, mp.c, lapic.c)
    FreeBSD (ioapic.c)
    NetBSD (console.c)

The following people have made contributions: Russ Cox (context switching,
locking), Cliff Frey (MP), Xiao Yu (MP), Nickolai Zeldovich, and Austin
Clements.

We are also grateful for the bug reports and patches contributed by Silas
Boyd-Wickizer, Mike CAT, Nelson Elhage, Nathaniel Filardo, Peter Froehlich,
Yakir Goaran, Shivam Handa, Bryan Henry, Jim Huang, Anders Kaseorg, kehao95,
Eddie Kohler, Imbar Marinescu, Yandong Mao, Hitoshi Mitake, Carmi Merimovich,
Joel Nider, Greg Price, Ayan Shafqat, Eldar Sehayek, Yongming Shen, Cam Tenny,
Rafael Ubal, Warren Toomey, Stephen Tu, Pablo Ventura, Xi Wang, Keiichi
Watanabe, Nicolas Wolovick, Jindong Zhang, and Zou Chang Wei.

The code in the files that constitute xv6 is
Copyright 2006-2016 Frans Kaashoek, Robert Morris, and Russ Cox.

ERROR REPORTS

If you spot errors or have suggestions for improvement, please send
email to Frans Kaashoek and Robert Morris (kaashoek,rtm@csail.mit.edu).

BUILDING AND RUNNING XV6

To build xv6 on an x86 ELF machine (like Linux or FreeBSD), run "make".
On non-x86 or non-ELF machines (like OS X, even on x86), you will
need to install a cross-compiler gcc suite capable of producing x86 ELF
binaries.  See http://pdos.csail.mit.edu/6.828/2016/tools.html.
Then run "make TOOLPREFIX=i386-jos-elf-".

To run xv6, install the QEMU PC simulators.  To run in QEMU, run "make qemu".

To create a typeset version of the code, run "make xv6.pdf".  This
requires the "mpage" utility.  See http://www.mesa.nl/pub/mpage/.


# LAB 4 CHANGES
To test the code, there are two tests that the user can run that have been added to the user programs that can be run in the shell:
1. `selecttest`: this program will run a number of tests that check to see if the select system call is actually functioning properly. There are 5 total tests:
	* `test0`: this test just checks if all of the logic to manipulate file descriptor sets is correct
	* `test1`: this test checks if selecting on pipes works correctly if you set the file descriptor sets beforehand
	* `test2`: this test checks if selecting on the console I/O works correctly if you set the file descriptor sets beforehand
	* `test3p`: this test checks if selecting on pipe works correctly when pipe hasn't been set to write prior
	* `test3`: this test checks if selecting on console works correctly when console hasn't been set to write prior. This test will block on console read until you enter something into the console followed by an ending character (i.e. `\n`)
All of these tests will print `TEST<#> SUCCESS!` if the test is successful, and all tests are run consecutively. Output will look something like this:

```
init: starting sh
$ selecttest
test0
TEST0 SUCCESS!

test1
TEST1 SUCCESS!

test2
Console read fd 0 not set
Console write fd 0 not set
Console read fd 1 not set
Console write fd 1 set
Console read fd 2 not set
Console write fd 2 set
correct: 5

test3p
SelSleeping for 100
ecting on Read
Writing to pipe
fd 0 not set to read
fd 0 not set to write
fd 1 not set to read
fd 1 not set to write
fd 2 not set to read
fd 2 not set to write
fd 3 set read
fd 3 not set to write
fd 4 not set to read
fd 4 not set to write
I am a write buffer
TEST3P SUCCESS!

test3

Console fd 0 set
Console write fd 0 not set
Console fd 0 not set to read
Console write fd 1 not set
Console fd 0 not set to read
Console write fd 2 not set
TEST3 SUCCESS!

test3
Console fd 0 set
Console write fd 0 not set
Console fd 0 not set to read
Console write fd 1 not set
Console fd 0 not set to read
Console write fd 2 not set
TEST3 SUCCESS!
$ $ 
```

2. `pipenet`: this creates a pipenet that allows for non-blocking multiplexed pipe and console input and output. In pipenet, this functionality is shown by piping commands written to the console (by reading from the console) into a pipe to the shell and then reading from that pipe once the shell returns and writing it to the console. Debug output is displayed to show this feature:

```
init: starting sh
$ pipenet


 ***** Starting PipeNet ***** 


Reading from pipe
$ ls

Reading from console

Reading from pipe
.              1 1 512
..             1 1 512
README         2 2 2191
cat            2 3 13108
echo 
Reading from pipe
          2 4 12328
forktest       2 5 8048
grep           2 6 14744
init           2 7 12880
kill  
Reading from pipe
         2 8 12400
ln             2 9 12296
ls             2 10 14520
mkdir          2 11 12452
rm  
Reading from pipe
           2 12 12428
sh             2 13 23232
stressfs       2 14 13020
usertests      2 15 58948

Reading from pipe
wc             2 16 13624
zombie         2 17 12104
pipenet        2 18 15028
selecttest     2 19 18
Reading from pipe
720
console        3 20 0
$
```

If selecttest were run inside of pipenet, the output would look something like this:

```
init: starting sh
$ pipenet


 ***** Starting PipeNet ***** 


Reading from pipe
$ selecttest

Reading from console

Reading from pipe
test0
TEST0 SUCCESS!

test1
TEST1 SUCCESS!

test2
Console read fd 0 not set
Console write fd 0 not s
Reading from pipe
et
Console read fd 1 not set
Console write fd 1 set
Console read fd 2 not set
Console write fd 2 set
Reading from pipe

correct: 5

test3p
Selecting on Read
Sleeping for 100

Reading from pipe
Writing to pipe
fd 0 not set to read
fd 0 not set to write
fd 1 not set to read
fd 1 not set to writ
Reading from pipe
e
fd 2 not set to read
fd 2 not set to write
fd 3 set read
fd 3 not set to write
fd 4 not set to rea
Reading from pipe
d
fd 4 not set to write
I am a write buffer
TEST3P SUCCESS!

test3


Reading from console

Reading from pipe
Console fd 0 set
Console write fd 0 not set
Console fd 0 not set to read
Console write fd 1 not set

Reading from pipe
Console fd 0 not set to read
Console write fd 2 not set
TEST3 SUCCESS!

test3
Console fd 0 set
Conso
Reading from pipe
le write fd 0 not set
Console fd 0 not set to read
Console write fd 1 not set
Console fd 0 not set t
Reading from pipe
o read
Console write fd 2 not set
TEST3 SUCCESS!
$ $ 
```
Note that `selecttest` will also wait on a console input. Also note that `test2` does not pass because `pipenet` closes `fd 0` or `stdin`.

## Files that have been changed
1. `console.c:301-367` - methods to facilitate the operation of `sys_select()` have been implemented, including:
	* `consolewriteable()` - Indicates if the console can be written without blocking (0 if true, -1 if error, otherwise its false). This always returns true because you can always write to the console
	* `consolereadable()` - Indicates if the console can be read without blocking (0 if true, -1 if error, otherwise its false). This first checks if the console has already read all that was written and if not, the console is readable (i.e. there are things to read)
	* `consoleselect()` - Adds the selid to be woken up using the `addselid()` function in `select.c`. This will only add to the read queue as nothing will wait on write and this queue will hold all the ips that are waiting to read.
	* `consoleclrsel()` - Removes the selid from being woken up using the `clearselid()` function in `select.c`. This will only clear the selid from the read queue because the console ip was only added to the read queue
2. `console.c:230` - in `consoleintr()`, if the console writes an end lline character `\n`, then it will wake up anything waiting on console read in the read queue.
3. `pipe.c:176-272` - methods to facilitate the operation of `sys_select()` have been implemented, just like in `console.c` including:
	* `pipewriteable()` - Indicates if the console can be written without blocking (0 if true, -1 if error, otherwise its false). This will check if the proc is kkilled (return -1), if the write buffer is full (return 1) and if both of those are false, return 0
	* `pipereadable()` - Indicates if the console can be read without blocking (0 if true, -1 if error, otherwise its false). This first checks if the proc is killed (return -1), and if there is nothing else to read (return 1), and if both of those are false, return 0
	* `pipeselect()` - Adds the selid to be woken up using the `addselid()` function in `select.c`. This will depend on the selid of the proc --- add to read queue if selid is 0, add to write queue if proc selid is 1
	* `pipeclrsel()` - Removes the selid from being woken up using the `clearselid()` function in `select.c`. This will do the same checks as `pipeselect()`, just calling `clearselid()` instead of `addselid()`
4. `pipe.c:125-162` - in `pipewrite()` and `piperead()`, once the read/write process is finished, the functions will then call `wakeupselect()` to wake up any ips waiting the queues to read/write
5. `select.c:72-74` - `wakeupselect()` changed so now the selid is now cleared from the queue after being chosen to wakeup and before actually waking it up
6. `sysfile.c:465-587` - the system call `sys_select()` was implemented. It first loops through the fd sets to check if you have to actually see if the fd is readable/writeable. If that is the case, the loop will verify each fd passed in is open and valid. If that is the case, `filereadable()`/`filewriteable()` is called to see if that fd is actually readable or writeable. If it is, that fd is set into the new fd set that select "outputs" (i.e. just passes back to the caller that passed it to `sys_select()`). At the end of the loop, if no fds were readable or writeable, `fileselect()` is called on each fd to set wakeup call on that fd (another loop). Then, the select function will sleep on the selectlock. Upon wakeup, the whole loop is reset and run again. This time, if there are fds that are readable or writeable, the loop is broken out of and all the newly selected fds are cleared from the queues. Finally, the method returns 0.
7. `selecttest.c` - print statements were added to each of the tests to show a success or a failure.
8. `pipenet.c` - the `pipenet()` method was completely overhauled to now continually loop and select on the given fds to read/write from the pipes/console. Pipenet also acts as a terminal.