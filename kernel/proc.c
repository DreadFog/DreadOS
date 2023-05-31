#include <n7OS/proc.h>
#include <malloc.h>
#include <stdio.h>
process_t *processes[NB_PROC];
process_t storing_table[NB_PROC];
int current_process_id;
int current_process_index;
extern void ctx_sw(uint32_t *old_ctx, uint32_t *new_ctx);
// Function to initialize the process table, is supposed to be called before any fork

void init_process_table(fnptr root_program)
{
    for (int i = 0; i < NB_PROC; i++)
    {
        processes[i] = NULL;
        storing_table[i].is_available = 1;
    }
    current_process_index = 0;
    current_process_id = 0;
    process_t *k = add_process("kernel", 0, root_program);
    k->state = RUNNING; // this one is currently running
    // retrieve the current stack top
    uint32_t *stack_top;
    uint32_t *stack_bottom;
    __asm__("movl %%esp, %0"
            : "=r"(stack_top));
    __asm__("movl %%ebp, %0"
            : "=r"(stack_bottom));
    // Save them to the process
    k->sp = stack_top;
    memcpy(k->stack, stack_bottom, STACK_SIZE * sizeof(uint32_t));
    processes[k->pid] = k;
}
int preempt_pid()
{
    for (int i = 0; i < NB_PROC; i++)
    {
        // find the first available process entry in the table
        if (storing_table[i].is_available)
        {
            storing_table[i].is_available = 0;
            return i;
        }
    }
    return -1;
}
process_t *add_process(const char *name, pid_t ppid, fnptr function)
{
    int pid = preempt_pid();
    if (pid == -1)
    {
        printf("No available pid\n");
        return NULL; // no available pid
    }
    process_t *process = &storing_table[pid];
    process->name = name;
    process->ppid = ppid;
    process->pid = pid;
    process->stack[STACK_SIZE - 1] = (uint32_t)function;
    process->sp = &process->stack[STACK_SIZE - 1];
    for (int i = 0; i < CTX_SIZE; i++)
    {
        process->ctx[i] = 0;
    }
    process->ctx[ESP] = (uint32_t)process->sp;
    process->state = READY;
    process->resources = NULL;
    return process;
}
void remove_process(process_t *process)
{
    processes[process->pid] = NULL;
    process->is_available = 1;
}
process_t *get_process_by_pid(pid_t pid)
{
    return processes[pid];
}

int exec_fork(const char *name, fnptr function)
{
    process_t *process = add_process(name, current_process_id, function);
    if (process == NULL)
    {
        return -1;
    }
    processes[process->pid] = process;
    return process->pid;
}

void print_processes()
{
    printf("\nPrinting processes\n");
    printf("----------\n");
    for (int i = 0; i < NB_PROC; i++)
    {
        if (processes[i] != NULL)
        {
            printf("Process %s, pid %d, ppid %d\n", processes[i]->name, processes[i]->pid, processes[i]->ppid);
            printf("Stack pointer %p\n", processes[i]->sp);
            printf("Registers: ");
            for (int j = 0; j < CTX_SIZE; j++)
            {
                printf("%d\n ", processes[i]->ctx[j]);
            }
            printf("----------\n");
        }
    }
}

void scheduler()
{ // scheduler that alternates between ready processes
    printf("Scheduler called by process of pid %d\n", current_process_id);
    process_t *current_process = processes[current_process_index];
    current_process->state = READY;
    // find the next process to run
    while (1)
    {
        current_process_index = (current_process_index + 1) % NB_PROC;
        if (processes[current_process_index] != NULL && processes[current_process_index]->state == READY)
        {
            ctx_sw(current_process->ctx, processes[current_process_index]->ctx);
            processes[current_process_index]->state = RUNNING;
            current_process_id = processes[current_process_index]->pid;
            break;
        }
    }
    // if we still didn't find any process, we keep the current one
}

void stop_current_process()
{
    process_t *current_process = processes[current_process_index];
    remove_process(current_process);
    scheduler();
}