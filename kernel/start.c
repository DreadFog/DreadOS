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
    for (int i = 0; i < 10  ; i++)
    {
        printf("%d\n", i);
    }
}
void kernel_start(void)
{
    init_handlers();
    init_timer(1000);
    clear_console();
    sti();
    init_process_table(kernel_start);
    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        change_color(GREEN, BLUE);
        // cette fonction arrete le processeur
        for (int i = 0; i < 3; i++)
        {
            printf("%d", i);
            //__asm__("int $50");
        }
        // console_putbytes_position("Test with offset", 17, 10, 40);
        /* if (example() == 1)
        {    set_layout();

            printf("Appel systeme example ok");
        } */
        // shutdown(1);
        // while(1);
        change_color(BLACK, WHITE);
        fork("compteur", compteur);
        print_processes();
        for (;;)
        {
            //printf("Hello World");
            hlt();
        }
    }
}


