#include <inttypes.h>
#include <n7OS/irq.h>
#include <stdio.h>
#include <n7OS/sys.h>
#define TASK_GATE 5
#define INTERRUPT_GATE 6
#define TRAP_GATE 7
extern void handler_IT_50();
extern void handler_syscall();
void init_irq_entry(int irq_num, uint32_t addr)
{
    idt_entry_t *entry = (idt_entry_t *)&idt[irq_num];
    entry->offset_inf = addr & 0xFFFF;
    entry->sel_segment = KERNEL_CS;
    entry->zero = 0;
    // P = 1, DPL = 0, S = 0, GateType = 6
    entry->type_attr = 0x8E;
    entry->offset_sup = (addr >> 16) & 0xFFFF;
}

void handler_50()
{
    printf("handler_50\n");
}
void init_handlers()
{
    init_syscall();
    init_irq_entry(50, (uint32_t)handler_IT_50);
}