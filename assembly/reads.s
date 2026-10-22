	.section .bss.
buffer: .skip 128

	.section .text

	
# STACK FRAME NOT PROPERLY SET UP (fix later)
readInt:
	pop %rbx
	# prepare syscall for reading stdin
	mov $0, %rax						# read mode
	mov $0, %rdi						# stdin
	lea buffer (%rip), %rsi	# load buffer
	mov $128, %rdx					# data length
	syscall

	mov %rax, %rbx					# store number of bytes read (from rax)

	# initialize registers for parsing
	mov $0, %rcx						# set index to 0
	mov $0, %rax						# clear accumulator
	lea buffer(%rip), %rdi	# load buffer
	dec %rbx								# decrement byte count (exclude newline)
	mov %rbx, %rsi					# move byte count to rsi

	call parseInt
	pop %rbx
	ret

parseInt:
	# even the professor says this looks weird
	movzbl (%rdi, %rcx, 1), %r8d	# load 1 byte from buffer

	sub $'0', %r8									# convert to ascii value by subtracting
																# decimal value from ascii val for 0

	imul $10, %rax								# left shift accumulator by 1
																# (multiply by 10)
	
	add %r8, %rax									# add digit to accumulator
	inc %rcx											# increment index

	cmp %rsi, %rcx								# see if there are chars left
																# by comparing index and buffer size
		
	jb parseInt										# if index is below buffer size
	ret



