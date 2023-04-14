#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <stdio.h>
#include <n7OS/irq.h>
#include <n7OS/console.h>
void kernel_start(void)
{
    init_handler();
    sti();
    // on ne doit jamais sortir de kernel_start
    while (1)
    {
        // cette fonction arrete le processeur
        clear_console();
        for (int i = 0; i < 1000; i++)
        {
            printf("%d", i);
            printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbcccccccccc\n");

            __asm__("int $50");
        }
        console_putbytes_position("Test with offset", 17, 10, 40);
        hlt();
    }
}
