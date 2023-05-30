#ifndef __SYSCALL_DEFS_H__
#define __SYSCALL_DEFS_H__

#define NB_SYSCALL 5
typedef int (*fn_ptr)();
int sys_example();
int sys_shutdown(int n);
int sys_write(const char *s, int len);
int sys_fork(const char *name, fnptr function);
int sys_exit();
extern fn_ptr syscall_table[NB_SYSCALL];

void add_syscall(int num, fn_ptr function);

#endif
