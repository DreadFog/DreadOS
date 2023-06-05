#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <syscall_defs.h>
void compteur(void)
{
    change_color(BLACK, WHITE);
    for (int i = 0; i < 100000; i++)
    {
        printf("First wave of Hello %d from other process of pid %d\n", i, getpid());
    }
    //sleep(1000);
    for (int i = 0; i < 500; i++)
    {
        printf("Second wave of Hello %d from other process of pid %d\n", i, getpid());
    }
    exit();
}