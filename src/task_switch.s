.intel_syntax noprefix

.globl switch_task
switch_task:
    mov eax, [esp+4]     # old
    mov edx, [esp+8]     # new

    mov [eax+36], esp    # save old esp
    mov esp, [edx+36]    # load new esp

    ret

.global task_first_switch
task_first_switch:
    # argument: [esp+4] = new esp value
    mov eax, [esp+4]
    mov esp, eax
    popa
    iret