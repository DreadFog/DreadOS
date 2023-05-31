#include <n7OS/sys.h>
#include "syscall_defs.h"
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <unistd.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#include  <n7OS/proc.h>
extern void handler_syscall();

void init_syscall()
{
  // ajout de la fonction de traitement de l'appel systeme
  add_syscall(NR_example, (void *)sys_example);
  add_syscall(NR_shutdown, (void *)sys_shutdown);
  add_syscall(NR_write, (void *)sys_write);
  add_syscall(NR_fork, (void *)sys_fork);
  add_syscall(NR_exit, (void *)sys_exit);
  // initialisation de l'IT soft qui gère les appels systeme
  init_irq_entry(0x80, (uint32_t)handler_syscall);
}

// code de la fonction de traitement de l'appel systeme example
int sys_example()
{
  // on ne fait que retourner 1
  return 1;
}
// Shutdown Syscall
int sys_shutdown(int n)
{
  if (n == 1)
  {
    outw(0x2000, 0x604);
    return -1;
  }
  else
  {
    return n;
  }
}

//write syscall
int sys_write(const char *s, int len)
{
  console_putbytes(s, len);
  return 1;
}
// fork syscall
int sys_fork(const char *name, fnptr function)
{
  return exec_fork(name, (void (*)(void))function);
}
// exit syscall
int sys_exit()
{
  printf("exit syscall\n");
  stop_current_process();
  return 1;
}
