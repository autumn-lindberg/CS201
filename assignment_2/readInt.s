	.section .bss
buffer: .skip 128

	.section .text
	.global readInt

readInt:
	push %rbx
	
	# set up read mode
	mov $0, %rax
	# stdin
	mov $0, %rdi
	# load buffer into rsi
	lea buffer (%rip), %rsi
	# data length
	mov $128, %rdx
	syscall

	# store number of bytes read (from rax)
	mov %rax, %rbx

	# set index to 0
	mov $0, %rcx
	# clear accumulator
	mov $0, %rax
	# load buffer into rdi
	lea buffer (%rip), %rdi
	# decrement byte count to exclude \n
	dec %rbx
	# move byte count to rsi
	mov %rbx, %rsi

	call parseInt

	pop %rbx
	ret

parseInt:
	# load 1 byte from bffer
	movzbl (%rdi, %rcx, 1), %r8d
	# convert to ascii by subtracting decimal value from 
	# ascii value of 0
	sub $'0', %r8
	# left shift accumulator by 1 (multiply by 10)
	shl $1, %rax
	# add to running total
	add %r8, %rax
	#increment index
	inc %rcx

	# see if there are chars left by comparing index and buffer size
	cmp %rsi, %rcx

	# jump if index is below byte count
	jb parseInt
	ret

