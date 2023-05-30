#ifndef __PROC_H__
#define __PROC_H__
#define NB_PROC 256
#define STACK_SIZE 4096
#define CTX_SIZE 5
#include <inttypes.h>
#include <unistd.h>
#include <stddef.h>
typedef enum { EBX, ESP, EBP, ESI, EDI } REG;
typedef uint32_t pid_t;
typedef enum {
    RUNNING,
    READY,
    BLOCKED
} PROCESS_STATE;
typedef struct resource_t {
    void *resource;
    struct resource_t *next;
} RESOURCE;
typedef struct process_t {
    const char *name;
    uint32_t stack[STACK_SIZE];
    uint32_t *sp; // stack pointer
    uint32_t ctx[CTX_SIZE];
    PROCESS_STATE state;
    pid_t pid;
    pid_t ppid;
    RESOURCE *resources;
    uint8_t is_available:1;
    //pid_t children[NB_PROC] ; // keep in mind the pid of the children. Idk if useful
} process_t;
void init_process_table();
process_t * add_process(const char *name, pid_t ppid, fnptr function);
void remove_process_by_pid(pid_t pid);
process_t *get_process_by_pid(pid_t pid);
#endif