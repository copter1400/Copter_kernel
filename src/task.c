#include "task.h"
#include "type.h"
#include "terminal.h"
#include "string.h"
#include "serial.h"

extern void task_first_switch(uint32_t new_esp);

#define MAX_TASKS 256

task_t tasks[MAX_TASKS];
int current_task = -1;   // -1 so first schedule() lands on task 0
uint32_t task_count = 0;
uint32_t next_pid = 0;

void task_exit() {
    tasks[current_task].active = 0;
    while(1) { asm volatile("hlt"); }  // wait for timer to switch away
}

void create_task(uint32_t entry, const char* name) {
    int idx = task_count++;
    task_t* t = &tasks[idx];
    t->active = 1;
    t->pid = next_pid++;
    memset(t->name, 0, 32);
    strncpy(t->name, name, 31);
    t->name[31] = '\0';

    uint32_t stack_top = 0x90000 - (idx * 0x2000);
    uint32_t* stack = (uint32_t*)stack_top;

    *(--stack) = 0x202;
    *(--stack) = 0x10;
    *(--stack) = entry;
    *(--stack) = 0;                    // eax
    *(--stack) = 0;                    // ecx
    *(--stack) = 0;                    // edx
    *(--stack) = 0;                    // ebx
    *(--stack) = (uint32_t)stack_top;  // esp
    *(--stack) = 0;                    // ebp
    *(--stack) = 0;                    // esi
    *(--stack) = 0;                    // edi

    t->regs.esp = (uint32_t)stack;

    *(--stack) = (uint32_t)task_exit;  // return address
    *(--stack) = t->pid;               // argument: pid
}

void kill_task(int pid) {
    for (uint32_t i = 0; i < task_count; i++) {
        if (tasks[i].pid == pid && tasks[i].active) {
            tasks[i].active = 0;
            print("killed pid ");
            print_int(pid);
            print("\n");
            // if we just killed ourselves, stop running
            if ((int)i == current_task) {
                while(1) { asm volatile("hlt"); }
            }
            return;
        }
    }
    print("no task exist with PID ");
    print_int(pid);
    print("\n");
}

uint32_t schedule_irq(uint32_t old_esp) {
    if (task_count == 0) return old_esp;
    
    // save current task esp only if we have a valid current task
    if (current_task != -1) {
        tasks[current_task].regs.esp = old_esp;
    }
    
    // find next active task
    int next = (current_task == -1) ? 0 : current_task;
    int start = next;
    do {
        next = (next + 1) % task_count;
    } while (!tasks[next].active && next != start);

    if (next == current_task) return old_esp;  // no other task

    current_task = next;
    return tasks[current_task].regs.esp;
}

                                                                                                                                                                                                                                                                       
void tls() {
    print("PID  NAME\n");
    print("---  ----\n");
    for (uint32_t i = 0; i < task_count; i++) {
        if (tasks[i].active) {
            print_int(tasks[i].pid);
            print("    ");
            print(tasks[i].name);
            print("\n");
        }
    }
}
