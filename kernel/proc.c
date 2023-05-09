#include <n7OS/proc.h>
#include <malloc.h>

PROCESS *processes[NB_PROC];
PROCESS *current_process;
int current_process_index;
// Function to initialize the process table, is supposed to be called before any fork
void init_process_table()
{
    for (int i = 1; i < NB_PROC; i++)
    {
        processes[i] = NULL;
    }
    processes[0] = create_process("kernel", 0); // the root process has no parent process
    current_process = 0;
}
int find_pid()
{
    for (int i = 0; i < NB_PROC; i++)
    {
        if (processes[i] == NULL)
        {
            return i;
        }
    }
    return -1;
}
PROCESS *create_process(const char *name,  pid_t ppid) {
    PROCESS *process = (PROCESS *)malloc(sizeof(PROCESS));
    process->name = name;
    process->sp = &process->stack[STACK_SIZE-1];
    for (int i = 0; i < CTX_SIZE; i++) {
        process->ctx[i] = 0;
    }
    process->ctx[ESP] = (uint32_t)process->sp;
    process->state = READY;
    int pid = find_pid();
    if (pid == -1)
    {
        return NULL; // no available pid
    }
    process->pid = pid;
    process->ppid = ppid;
    process->resources = NULL;
   /*  for (int i = 0; i < NB_PROC; i++)
    {
        process->children[i] = -1;
    } */
    return process;
}
/*void add_process(PROCESS *process);
void remove_process(PROCESS *process);
void remove_process_by_pid(pid_t pid);
PROCESS *get_process_by_pid(pid_t pid);
*/