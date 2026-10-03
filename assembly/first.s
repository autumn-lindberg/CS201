# section text means "this is code"
	.section .text

	# make start "public"
	.global _start

# "main" section of assembly
_start:

	# put 10 into rcx
	mov $10, %rcx

	# put 20 into rbx
	mov $20, %rbx

	# rcx = rcx + rbx
	# add to rcx and store back into it
	add %rbx, %rcx

	# put 60 (code to end program) into %rax (system calls)
	mov $60, %rax

	syscall
