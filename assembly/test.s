	.section .bss
buffer:
	.zero 4

	.section .text
	.global _start
	.extern nopsled # this file depends on this function


vulnerable:

.Lcopy:
		testl   %ecx, %ecx
		je      .Lterminate           # buffer full: add null terminator
		movb    (%rsi), %al
		movb    %al, (%rdi)
		incq    %rsi
		incq    %rdi
		decl    %ecx
		testb   %al, %al
		jne     .Lcopy
		jmp			.Lexit
.Lterminate:
		movb    $0, (%rdi)            # ensure buffer is null-terminated

.Lexit:
		

		mov $60, %rax          # exit syscall (execve failed)
		syscall

_start:

		movq 16(%rsp), %rsi
    leaq buffer (%rip), %rdi        # Buffer address
    mov $64, %ecx 								# Intentionally excessive read size

		call vulnerable


