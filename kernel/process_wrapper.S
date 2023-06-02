.text
.globl process_wrapper
process_wrapper:
    # Save the return address
    pushl %ebp
    movl %ebp, %esp

    # Call the function in ebx
    call *(%ebx)

    # Clean up the stack
    movl %esp, %ebp
    popl %ebp

    # Invoke the exit syscall
    movl %eax, 4  # Syscall number for exit
    xor %ebx, %ebx  # Exit status 0
    int $0x80  # Invoke the syscall

    # Should not reach here
    ret
