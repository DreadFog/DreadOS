#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <syscall_defs.h>
void compteur(void)
{
    change_color(BLACK, WHITE);
    for (int i = 0; i < 10000; i++)
    {
        printf("Hello %d from other process of pid %d\n", i, getpid());
    }
    exit();
}