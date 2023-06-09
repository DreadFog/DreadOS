#include <n7OS/proc.h>
process_t *processes[NB_PROC];
process_t storing_table[NB_PROC];
sleeping_process_t *sleeping_processes = NULL;
int current_process_id;
int current_process_index;
extern void ctx_sw(uint32_t *old_ctx, uint32_t *new_ctx);
extern void process_wrapper();
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
    process_t *k = add_process("kernel", 0, NULL);
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
void add_to_pointer_list(int pid) {
    for (int i = 0; i < NB_PROC; i++) {
        if (processes[i] == NULL) {
            processes[i] = &storing_table[pid];
            break;
        }
    }
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
{ // call the wrapper add_process and put to ebx the address of the function
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
    process->stack[STACK_SIZE - 1] = (uint32_t) function;
    process->sp = &process->stack[STACK_SIZE - 1];
    for (int i = 0; i < CTX_SIZE; i++)
    {
        process->ctx[i] = 0;
    }
    process->ctx[ESP] = (uint32_t)process->sp;
    //process->ctx[EBX] = (uint32_t) function;
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
    printf("==========\n");
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
            printf("==========\n");
        }
    }
}

void scheduler()
{ // scheduler that alternates between ready processes
    printf("Scheduler called while process of pid %d is active\n", current_process_id);
    process_t *current_process = processes[current_process_index];
    if (current_process != NULL)
    { // could be null when we call schedule just after the main process has been suspended
        current_process->state = READY;
    }
    // find the next process to run
    while (1)
    {
        current_process_index = (current_process_index + 1) % NB_PROC;
        if (processes[current_process_index] != NULL && processes[current_process_index]->state == READY)
        {
            processes[current_process_index]->state = RUNNING;
            current_process_id = processes[current_process_index]->pid;
            ctx_sw(current_process->ctx, processes[current_process_index]->ctx);
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
void block_current_process()
{
    process_t *current_process = processes[current_process_index];
    current_process->state = BLOCKED;
    scheduler();
}
void unblock_process(pid_t pid)
{
    storing_table[pid].state = READY;
    add_to_pointer_list(pid);   
}
int get_current_process_id()
{
    return current_process_id;
}

// =================== //
// ==Sleeping processes ==//
// =================== //
void print_sleeping_processes()
{
    printf("\nPrinting sleeping processes\n");
    printf("==========\n");
    sleeping_process_t *current = sleeping_processes;
    while (current != NULL)
    {
        printf("Process %d, wake up time %d\n", current->pid, current->wake_up_time);
        current = current->next;
    }
    printf("==========\n");
}
void add_sleeping_process_list(pid_t pid, uint32_t wake_up_time, sleeping_process_t *list)
{
    sleeping_process_t *tail = list->next;
    if (tail == NULL)
    {
        sleeping_process_t *new = (sleeping_process_t *)malloc(sizeof(sleeping_process_t));
        new->pid = pid;
        new->wake_up_time = wake_up_time;
        list->next = new;
    }
    else if (tail->wake_up_time >= wake_up_time)
    {
        // reduce the tail wait time by wake_up_time
        tail->wake_up_time -= wake_up_time;
        // put the new process as first element of the tail
        sleeping_process_t *new = (sleeping_process_t *)malloc(sizeof(sleeping_process_t));
        new->pid = pid;
        new->wake_up_time = wake_up_time;
        list->next = new;
        new->next = tail;
    }
    else
    { // decrement the relative time to sleep and go into recursion
        add_sleeping_process_list(pid, wake_up_time - list->wake_up_time, sleeping_processes);
    }
}
void add_sleeping_process(pid_t pid, uint32_t wake_up_time)
{
    printf("Adding process %d to sleep for %d\n", pid, wake_up_time);
    sleeping_process_t *new = (sleeping_process_t *)malloc(sizeof(sleeping_process_t));
    printf("malloc'd\n");
    new->pid = pid;
    new->wake_up_time = wake_up_time;
    if (sleeping_processes == NULL)
    {
        sleeping_processes = new;
    }
    else if (sleeping_processes->wake_up_time >= wake_up_time) // new process shall be put first
    {
        // reduce the tail wait time by wake_up_time
        sleeping_processes->wake_up_time -= wake_up_time;
        sleeping_process_t *tail = sleeping_processes;
        // put the new process as first
        sleeping_processes = new;
        new->next = tail;
    }
    else
    { // decrement the relative time to sleep and go into recursion
        add_sleeping_process_list(pid, wake_up_time - sleeping_processes->wake_up_time, sleeping_processes);
    }
}
int do_sleep(int time)
{
    process_t *current_process = processes[current_process_index];
    current_process->state = SLEEPING;
    processes[current_process_index] = NULL;
    add_sleeping_process(current_process->pid, time);
    scheduler();
    return 0;
}
void update_sleeping_processes()
{ // check the time to sleep, if it is equal to zero add it back to the list of ready processes
    sleeping_process_t *head = sleeping_processes;
    if (head == NULL)
    {
        return; // no sleeping processes
    }
    if (head->wake_up_time == 0 ) {
        // wake it up, remove it from the list and add it to the ready processes
        sleeping_processes = head->next;
        unblock_process(head->pid);
        //free(head);
    } else {
        head->wake_up_time--;
    }
}