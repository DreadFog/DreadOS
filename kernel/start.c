#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <stdio.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>
#include <unistd.h>
#include <n7OS/time.h>
#include <stddef.h>
#include <n7OS/proc.h>
#define NB_ITER 5
void compteur(void)
{
    change_color(BLACK, WHITE);
    for (int i = 0; i < NB_ITER; i++)
    {
        printf("Hello %d from other process of pid %d\n", i, getpid());
        scheduler();
    }
    exit();
}
void kernel_start(void)
{
    init_handlers();
    init_timer(1000);
    sti();
    clear_console();
    init_process_table(kernel_start);
    change_color(GREEN, BLUE);
    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        fork("compteur1", compteur);
        for (int i = 0; i < NB_ITER; i++)
        {
            printf("Hello %d from kernel of pid %d  \n", i, getpid());
            // print_processes();
            scheduler();
        }
        for (;;)
        {
            hlt();
        }
    }
}
