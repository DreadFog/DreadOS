#include <n7OS/proc.h>
#include <malloc.h>
#include <stdio.h>
process_t *processes[NB_PROC];
process_t storing_table[NB_PROC];
int current_process_index;
// Function to initialize the process table, is supposed to be called before any fork
void init_process_table(fnptr root_program)
{
    for (int i = 1; i < NB_PROC; i++)
    {
        processes[i] = NULL;
        storing_table[i].is_available = 1;
    }
    add_process("kernel", 0, root_program);
    current_process_index = 0;
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
    printf("Adding process %s\n", name);
    int pid = preempt_pid();
    if (pid == -1)
    {
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
        processes[pid]->ctx[i] = 0;
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
/* int fork(const char *name, fnptr function)
{
    process_t *process = add_process(name, current_process_index, function);
    if (process == NULL)
    {
        return -1;
    }
    processes[process->pid] = process;
    return process->pid;
} */