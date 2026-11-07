.section .text
.global _start
.extern nopsled 

_start:
		# load pointer of argv[1] into rsi (the pointer of what to read)
		movq 16(%rsp), %rsi

		# CALL function (creates a stack)
    call vulnerable

		# exit
    mov $60, %rax
    xor %rdi, %rdi
    syscall

vulnerable:
    # create a stack frame 
    pushq %rbp
    movq %rsp, %rbp

    # allocate a 4-byte buffer ON THE STACK 
    subq $4, %rsp 

		# redirect destination of input (%rdi) is now the STACK buffer
    movq %rsp, %rdi 

    # this loop copies bytes from %rsi (same as we've been doing with stdin)
		# but rsi now contains argv[1])
		#
		# it copies them to %rdi even if it exceeds the size of 
		# the allocated buffer
.Lcopy_loop:
		# load byte from rsi to general register
    movb (%rsi), %al
		# store byte in destination (THIS IS WHERE THE OVERFLOW CAN HAPPEN)
		# THE DESTINATION IS THE BUFFER ON THE STACK
    movb %al, (%rdi)
		# move the source and destination pointers forward
    incq %rsi
    incq %rdi
		# stop only when we hit a null terminator (\0)
    testb %al,%al
		jne .Lcopy_loop

    # tear down stack frame and return 
    movq %rbp, %rsp
    popq %rbp

    # pops the overwritten stack address into RIP!
		#
		ret

.section .data
    .quad nopsled       

