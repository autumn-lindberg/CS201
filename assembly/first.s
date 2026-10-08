# 64-bit	32-bit	16-bit	8-bit	Typical use
# %rax    %eax	  %ax	    %al   accumulator (mul, div, etc)
# %rbx	  %ebx	  %bx	    %bl	  base (array base address)
# %rcx	  %ecx	  %cx	    %cl   counter (loop / rep instructions)
# %rdx	  %edx	  %dx	    %dl 	data (I/O operations, mul, div)
# %rsi	  %esi	  %si	    %sil	source index for string operations
# %rdi	  %edi	  %di	    %dil	destination index for string operations (PROGRAM RETURN VAL)
# %rbp	  %ebp	  %bp	    %bpl	Base pointer (DONT TOUCH)
# %rsp	  %esp	  %sp	    %spl	Stack pointer (DONT TOUCH)
# %r8	    %r8d	  %r8w	  %r8b	5th argument
# %r9	    %r9d	  %r9w	  %r9b	6th argument
# %r10	  %r10d	  %r10w	  %r10b	Caller-saved
# %r11	  %r11d	  %r11w	  %r11b	Caller-saved
# %r12	  %r12d 	%r12w	  %r12b	Callee-saved
# %r13	  %r13d  	%r13w	  %r13b	Callee-saved
# %r14	  %r14d	  %r14w	  %r14b	Callee-saved
# %r15	  %r15d	  %r15w	  %r15b	Callee-saved
# %rip                          pseudo-register (positional addresses)

# store variables here
				.section .data	# quad means 64-bit (long)
												# long means 32-bit (int)
num:		.quad 200				# c equivalent: long num = 200

# arrays
nums:		.quad 20, 30, 40, 50


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
				addq $16, (%rbx, %rax, 8)		# c equivalent: rbx += *(rax) * sizeof(long));
																		#								*(rbx) += 16;

# put into return value of program (ONE unsigned byte)
				movq (%rbx, %rax, 8), %rdi

# comparisons
				cmp %rcx, %rax		# if(rax - rcx > 0) go to myJump; 
													# 2 - 3 >= 0 is FALSE, don't jump
				jge MYJUMP				# if it's greater, jump to "MYJUMP" section
													# jg, jl, jle, je, jne, jz, jnz

				# under here is the "else" section
				mov $1, %rdi
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
				mov $60, %rax
				syscall
