	.section .data

# difference between current address and msg gives length in bytes
prompt:	.ascii "Enter an integer: "
prompt_length = . - prompt

before: .ascii "Your array before bubble sort: "
before_length = . - before

after:	.ascii "Your array after bubble sort: "
after_length = . - after

space:	.byte 32
newline: .byte 10

array: .quad 0, 0, 0, 0, 0

	.section .text
	.global _start
	.extern readInt
	.extern writeInt

evaluate_loop:
	# evaluate loop condition
	add $1, %r12
	cmp $4, %r12
	jl single_pass
	ret
	
single_pass:
	# compare adjacent elements
	mov (%rbx, %r12, 8), %r11
	cmp 8(%rbx, %r12, 8), %r11
	jle evaluate_loop

	# put lesser num in placeholder, put on stack
	mov 8(%rbx, %r12, 8), %r10
	# put greater num in greater spot
	mov %r11, 8(%rbx, %r12, 8)
	# put lesser num in lesser spot
	mov %r10, (%rbx, %r12, 8)
	
	# continue checking loop after swapping
	jmp evaluate_loop

bubble_sort:
	mov $0, %r12
	call single_pass

	# evaluate loop condition
	add $1, %r9
	cmp $4, %r9
	jl bubble_sort
	ret

write_before:
	# prepare arguments
	mov $1, %rax
	mov $1, %rdi
	# load buffer and length
	lea before (%rip), %rsi
	mov $before_length, %rdx
	syscall
	ret

write_after:
	# prepare arguments
	mov $1, %rax
	mov $1, %rdi
	# load buffer and length
	lea after (%rip), %rsi
	mov $after_length, %rdx
	syscall
	ret

write_prompt:
	# prepare arguments
	mov $1, %rax
	mov $1, %rdi
	# load buffer and length
	lea prompt (%rip), %rsi
	mov $prompt_length, %rdx
	syscall
	ret

read_and_store:
	call write_prompt
	call readInt

	# store result in array
	mov %rax, (%rbx, %r12, 8)
	inc %r12
	ret

display:
	# move array into rdi so it can be printed
	mov (%rbx, %r12, 8), %rdi
	call writeInt
	# print a space
	mov $1, %rax
	mov $1, %rdi
	lea space (%rip), %rsi
	mov $1, %rdx
	syscall

	# evaluate loop condition
	add $1, %r12
	cmp $5, %r12
	jl display

	# print a new line
	mov $1, %rax
	mov $1, %rdi
	lea newline (%rip), %rsi
	mov $1, %rdx
	syscall

	ret

exit:
	mov $60, %rax
	# xor %rdi, %rdi
	syscall

_start:
	# set index to 0
	mov $0, %r12

	# load array into rbx
	lea array (%rip), %rbx
	
	call read_and_store
	call read_and_store
	call read_and_store
	call read_and_store
	call read_and_store

	# reset index for display
	mov $0, %r12
	call write_before
	call display

	call bubble_sort

	# reset index for display
	mov $0, %r12
	call write_after
	call display

	jmp exit
