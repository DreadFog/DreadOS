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
void compteur(void)
{
    change_color(BLACK, WHITE);
    for (int i = 0; i < 10000; i++)
    {
        printf("Hello from other process %d\n", i);
        //scheduler();
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
    printf("Hello from kernel\n");
    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        fork("compteur", compteur);
        fork("compteur", compteur);
        fork("compteur", compteur);
        for (int i = 0; i < 10000; i++)
        {
            printf("Hello from kernel %d\n", i);
            //scheduler();
        }
        // print_processes();
        for (;;)
        {
            hlt();
        }
    }
}
