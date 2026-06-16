#ifndef TASK_H
#define TASK_H
#include "type.h"

typedef struct {
    uint32_t edi;      // 0
    uint32_t esi;      // 4
    uint32_t ebx;      // 8
    uint32_t ecx;      // 12
    uint32_t eax;      // 16
    uint32_t edx;      // 20
    uint32_t padding;  // 24  (was eax slot, keep struct size same)
    uint32_t eip;      // 28
    uint32_t eflags;   // 32
    uint32_t esp;      // 36
} task_regs_t;

typedef struct {
    task_regs_t regs;
    int active;
    int pid;
    char name[32];
} task_t;

extern task_t tasks[];
extern int current_task;
extern uint32_t task_count;

uint32_t schedule_irq(uint32_t old_esp);
void create_task(uint32_t entry, const char* name);
void switch_task(task_regs_t* old, task_regs_t* new);
void kill_task(int pid);
void tls();

#endif