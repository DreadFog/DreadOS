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
#define NB_ITER 10000
extern void compteur();
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
        fork("compteur2", compteur);

        for (int i = 0; i < NB_ITER; i++)
        {
            //printf("Hello %d from kernel of pid %d  \n", i, getpid());
            print_processes();
            //shutdown(1);
        }
        for (;;)
        {
            hlt();
        }
    }
}
/*
Ce qui marche:
- Syscalls (exit, fork, getpid,  shutdown, write)
- Interruptions
- Gestion des processus (création, fin, affichage, scheduler)
- Gestion de la console (affichage, changement de couleur, effacement)

Ce qui ne marche pas:
- Suspension des processus (sleep) à cause d'une erreur malloc
*/
