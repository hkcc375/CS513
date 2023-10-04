/*

When we call strace -c mkfifo myfifofile

% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
100.00    0.000118          11        10           close
  0.00    0.000000           0         1           mknodat
  0.00    0.000000           0         2         2 statfs
  0.00    0.000000           0         2         2 faccessat
  0.00    0.000000           0         8           openat
  0.00    0.000000           0         7           read
  0.00    0.000000           0         8           fstat
  0.00    0.000000           0         1           set_tid_address
  0.00    0.000000           0         1           set_robust_list
  0.00    0.000000           0         2           rt_sigaction
  0.00    0.000000           0         1           rt_sigprocmask
  0.00    0.000000           0         3           brk
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         1           execve
  0.00    0.000000           0        17           mmap
  0.00    0.000000           0        12           mprotect
  0.00    0.000000           0         1           prlimit64
------ ----------- ----------- --------- --------- ----------------
100.00    0.000118           1        78         4 total

When we call strace -c mknod myfifofile p

% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
  0.00    0.000000           0         1           mknodat
  0.00    0.000000           0         2         2 statfs
  0.00    0.000000           0         2         2 faccessat
  0.00    0.000000           0         8           openat
  0.00    0.000000           0        10           close
  0.00    0.000000           0         7           read
  0.00    0.000000           0         8           fstat
  0.00    0.000000           0         1           set_tid_address
  0.00    0.000000           0         1           set_robust_list
  0.00    0.000000           0         2           rt_sigaction
  0.00    0.000000           0         1           rt_sigprocmask
  0.00    0.000000           0         3           brk
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         1           execve
  0.00    0.000000           0        17           mmap
  0.00    0.000000           0        12           mprotect
  0.00    0.000000           0         1           prlimit64
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000           0        78         4 total

When we execute strace -c mkfifo myfifofile and strace -c mknod myfifofile p,
we can see that mkfifo internally calls mknodat system call because mkfifo is a
library function and that mkfifo is layered on top of mknod system call and does
some little additional work. Hence, mknod system call is definitely faster than
mkfifo library function.

Note : strace -c gives the taken by each system call while executing the given
command as well as the no. of times each system call is invoked.

*/