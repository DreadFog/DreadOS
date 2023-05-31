#ifndef __SYSCALL_DEFS_H__
#define __SYSCALL_DEFS_H__
#include <unistd.h>
#define NB_SYSCALL 6
int sys_example();
int sys_shutdown(int n);
int sys_write(const char *s, int len);
int sys_fork(const char *name, fnptr function);
int sys_exit();
int sys_getpid();
extern fnptr syscall_table[NB_SYSCALL];

void add_syscall(int num, fnptr function);

#endif
