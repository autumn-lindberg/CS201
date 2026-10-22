	.section .text
	.global _start

_start:
	mov $20, %rdi
	call sub 
	call sub
	call stack_frame
	call test_mult

	# end program
	mov %rax, %rdi
	mov $60, %rax
	syscall

# IF VALUE NEEDS TO BE SAVED, SUBROUTINE
# IF NO RETURN VALUE, USE JUMP / JUMP BACK

				# cmp %eax, %ebx
				# je EQUAL
				# // else
				# jmp END_EQUAL
	# EQUAL:
				# // do stuff
	# END_EQUAL:
				# // continue


# subroutines (this one doubles rdi) RETURNS GO INTO RAX 
sub:
	mov %rdi, %r10			# move passed argument into generic register (r10+)
	add %r10, %r10
	mov %r10, %rax
										# instruction pointer is saved on the stack
	ret								# ret pops the saved instruction pointer (rip)
										# and jmp to it

# stack frames (y = a + 2 * x)
stack_frame:
	push %rbp					# push OLD base pointer onto stack
	mov %rsp, %rbp		# reset the base pointer (to TOP of OLD stack)

	# make a new stack with room for 2 longs 
	# move stack pointer to TOP of NEW stack
	# STACK SIZE EXPECTS MULTIPLES OF 16
	sub $16, %rsp

	mov %rdi, -8(%rbp)			# push first argument onto new stack
	addq %rdi, -8(%rbp)			# double first argument
	movq -8(%rbp), %rax			# move result into rax
	movq $20, -16(%rbp)			# push 20 onto new stack (second local var)
	addq -16(%rbp), %rax		# add them together
	mov %rbp, %rsp					# return stack pointer to base
	pop %rbp
	ret

# multiplication (factorial)
test_mult:
	ret	

