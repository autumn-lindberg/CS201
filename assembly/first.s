# 64-bit	32-bit	16-bit	8-bit	Typical use
# %rax    %eax	  %ax	    %al   accumulator (returns, result of mul/div/etc, syscalls)
# %rbx	  %ebx	  %bx	    %bl	  base (array base address)
# %rcx	  %ecx	  %cx	    %cl   counter (loop / rep instructions)
# %rdx	  %edx	  %dx	    %dl 	data length (I/O operations)
# %rsi	  %esi	  %si	    %sil	source index for string operations
# %rdi	  %edi	  %di	    %dil	destination index (stdin, stdout, PROGRAM RETURN VAL)
# %rbp	  %ebp	  %bp	    %bpl	Base pointer (DONT TOUCH)
# %rsp	  %esp	  %sp	    %spl	Stack pointer 
# %r8	    %r8d	  %r8w	  %r8b	5th argument 
# %r9	    %r9d	  %r9w	  %r9b	6th argument 
# %r10	  %r10d	  %r10w	  %r10b CALLER SAVED (no need to save these in funcs)
# %r11	  %r11d	  %r11w	  %r11b	CALLER SAVED 
# %r12	  %r12d 	%r12w	  %r12b	free use / CALLEE SAVED
# %r12	  %r12d 	%r12w	  %r12b		if a function uses these,
# %r13	  %r13d  	%r13w	  %r13b		if a function uses these,        stack
# %r14	  %r14d	  %r14w	  %r14b		it must save their values to the  state
# %r15	  %r15d	  %r15w	  %r15b		and return them to their original
#                                 before returning (by convention)
#
# %rip                          pseudo-register (positional addresses)

# store variables here
				.section .data	# quad means 64-bit (long)
												# long means 32-bit (int)
num:		.quad 200				# c equivalent: long num = 200
arr:		.byte 97, 10
# ascii
msg:		.ascii "Hello world\n"
msg_length = . - msg		# difference between current address (.) and
												# address of msg (gives length in bytes)
												# (each char is a byte)
# arrays
nums:		.quad 20, 30, 40, 50

# UNINITIALIZED variables here
				.section .bss

buff:		.skip 128				# allocate 128 bytes of storage for this variable


# section text means "this is code"
				.section .text
# make start "public"
				.global _start


# "main" section of assembly
_start:

# add to rcx and store back into it
				mov $2, %rax									
				mov $1, %rcx
				add %rax, %rcx		# rcx = rcx + rax

# load effective address (equivalent of & operator)
																# long num = 200;
				lea num (%rip), %rbx		# rbx = &num;
				addq $10, (%rbx)				# *rbx = *rbx + 10;

																# () means dereference

# pointer math to get elements of array
				lea nums (%rip), %rbx				
				mov $2, %rax									
				addq $16, (%rbx, %rax, 8)		# c equivalent:

																		# rbx += *(rax) * sizeof(long));
																		# *(rbx) += 16;

																		# offset(base, index, sizeof(base))
																		# lea doesn't use memory

# put into return value of program (ONE unsigned byte)
				movq (%rbx, %rax, 8), %rdi

# stack (pushing / popping) - LOCAL VARIABLES
				pushq $20
				pushq $30
				pushq $10
				addq $8, %rsp			# offset 8 bits to get to next item in stack
				mov 8(%rsp), %rcx	# offset another 8 bits, put into rcx
				# popq %rcx

# comparisons
				cmp %rcx, %rax		# if(rax - rcx > 0) go to myJump; 
													# 2 - 20 >= 0 is FALSE, don't jump
				jge MYJUMP				# if it's greater, jump to "MYJUMP" section
													# jg, jl, jle, je, jne, jz, jnz

				# under here is the "else" section
# comparisons
				cmp %rcx, %rax		# if(rax - rcx > 0) go to myJump; 
													# 2 - 20 >= 0 is FALSE, don't jump
				jge MYJUMP				# if it's greater, jump to "MYJUMP" section
													# jg, jl, jle, je, jne, jz, jnz

				# under here is the "else" section

# reading / writing
													# argument order: rdi, rsi, rdx, rcx, r8, r9
													# anything else has to be passed on the stack

															# syscall for read is 0
				mov $1, %rax					# syscall for write is 1
					
															# stdin = 0
															# stdout = 1
															# stderror = 2
				
				mov $1, %rdi					# 1st argument (%rdi) is where to write to
															# (stdout)

				lea msg (%rip), %rsi	# 2nd argument (%rsi) is the POINTER of
															# what to write (arr OR msg)

				mov $msg_length, %rdx	# 3rd arugment (%rdx) is the length of data
															# to be received (string length) 
				syscall

				# writing
				mov $0, %rax
				mov $0, %rdi
				lea buff (%rip), %rsi
				mov $128, %rdx
				syscall

				# rax contains number of bytes actually read by stdin (length)
				# because accumulator stores results
				mov %rax, %rdx				# change data length (before rax is reused)
				mov $1, %rax					# change to write
				mov $1, %rdi					# change to stdout
				lea buff (%rip), %rsi
				syscall

				jmp CHICKEN

# loops
				mov $0, %rax			# set sum to 0
MYLOOP:
				add %rcx, %rax		# rcx is already 1, add to sum
				add $1, %rcx			# increment
				cmp $10, %rcx			# see if it's less than or equal to 10
				jle MYLOOP				# jump to top if it is

				mov %rax, %rdi

MYJUMP:
# put 60 (code to end program) into %rax (system calls)
				mov $60, %rax
				syscall
CHICKEN:
				mov %rcx, %rdi
				# zero out the program (no notes)
				# xor %rdi, %rdi
				mov $60, %rax
				syscall
