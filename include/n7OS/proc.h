#ifndef __PROC_H__
#define __PROC_H__
#define NB_PROC 256
#include <inttypes.h>
#include <unistd.h>
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
struct process_t {
    const char *name;
    void *stack;
    PROCESS_STATE state;
    pid_t pid;
    pid_t ppid;
    RESOURCE *resources;
    pid_t children[NB_PROC] ; // keep in mind the pid of the children. Idk if useful
};
//struct process_t *processes[NB_PROC];
#endif