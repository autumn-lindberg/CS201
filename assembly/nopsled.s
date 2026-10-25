
.section .rodata
sled_msg:
    .ascii "Entered NOP sled\n"
sled_msg_len = . - sled_msg

after_msg:
    .ascii "Passed NOP sled\n"
after_msg_len = . - after_msg

	.section .text
	.global nopsled

# do nothing but increment addresses
# if program execution happens (%rip falling inside of)
# ANYWHERE in the empty addresses, 
# 

nopsled:

	# Print entry marker
	mov $1, %rax
	mov $1, %rdi
	lea sled_msg(%rip), %rsi
	mov $sled_msg_len, %rdx
	syscall

	.rept 1000000
	nop
	.endr

	# Print exit marker
	mov $1, %rax
	mov $1, %rdi
	lea after_msg(%rip), %rsi
	mov $after_msg_len, %rdx
	syscall

	ret 
