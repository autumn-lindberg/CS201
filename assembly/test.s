	.section .text
	.global _start
	.extern readInt		# this file depends on this function

_start:
	call readInt

	mov $60, %rax
	xor %rdi, %rdi
	syscall
