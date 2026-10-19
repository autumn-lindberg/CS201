	.file	"basics.c"
	.text
	.globl	MAX_LENGTH
	.data
	.align 4
	.type	MAX_LENGTH, @object
	.size	MAX_LENGTH, 4
MAX_LENGTH:
	.long	100
	.section	.rodata
.LC0:
	.string	"The size of an int is: %ld\n"
.LC1:
	.string	"The size of a long is: %ld\n"
.LC2:
	.string	"The size of a char is: %ld\n"
.LC3:
	.string	"The size of a float is: %ld\n"
.LC4:
	.string	"The size of a double is: %ld\n"
.LC5:
	.string	"The size of an int * is: %ld\n"
.LC6:
	.string	"The size of a Rect is: %ld\n"
	.align 8
.LC7:
	.string	"The highest possible integer (using macro MAX_INT):"
.LC8:
	.string	"%d\n"
	.align 8
.LC9:
	.string	"The lowest possible integer is: %d\n"
	.align 8
.LC10:
	.string	"INT_MAX + 1 = %d (overflows to negative due to leading 1)\n"
	.align 8
.LC11:
	.string	"UINT_MAX + 1 = %d (ignores leading 1, rest of bits are 0)\n"
.LC12:
	.string	"Enter an integer: "
	.align 8
.LC13:
	.string	"Invalid input. Enter an integer: "
.LC14:
	.string	"%d"
.LC15:
	.string	"You entered %d\n"
.LC16:
	.string	"Enter your name: "
.LC17:
	.string	"%[^\n]"
.LC18:
	.string	"you entered %s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	leaq	-36(%rbp), %rax
	movq	%rax, -8(%rbp)
	movl	MAX_LENGTH(%rip), %eax
	cltq
	movq	%rax, %rdi
	call	malloc
	movq	%rax, -16(%rbp)
	movl	$4, %esi
	movl	$.LC0, %edi
	movl	$0, %eax
	call	printf
	movl	$8, %esi
	movl	$.LC1, %edi
	movl	$0, %eax
	call	printf
	movl	$1, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	call	printf
	movl	$4, %esi
	movl	$.LC3, %edi
	movl	$0, %eax
	call	printf
	movl	$8, %esi
	movl	$.LC4, %edi
	movl	$0, %eax
	call	printf
	movl	$8, %esi
	movl	$.LC5, %edi
	movl	$0, %eax
	call	printf
	movl	$16, %esi
	movl	$.LC6, %edi
	movl	$0, %eax
	call	printf
	movl	$10, %edi
	call	putchar
	movl	$.LC7, %edi
	movl	$0, %eax
	call	printf
	movl	$2147483647, %esi
	movl	$.LC8, %edi
	movl	$0, %eax
	call	printf
	movl	$-2147483648, %esi
	movl	$.LC9, %edi
	movl	$0, %eax
	call	printf
	movl	$-2147483648, %esi
	movl	$.LC10, %edi
	movl	$0, %eax
	call	printf
	movl	$0, %esi
	movl	$.LC11, %edi
	movl	$0, %eax
	call	printf
	movl	$.LC12, %edi
	movl	$0, %eax
	call	printf
	jmp	.L2
.L3:
	call	getchar
	movl	$.LC13, %edi
	movl	$0, %eax
	call	printf
.L2:
	leaq	-36(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC14, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	testl	%eax, %eax
	je	.L3
	movl	-36(%rbp), %eax
	movl	%eax, %esi
	movl	$.LC15, %edi
	movl	$0, %eax
	call	printf
	call	getchar
	movl	$.LC16, %edi
	movl	$0, %eax
	call	printf
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC17, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	call	getchar
	movq	-16(%rbp), %rax
	movq	%rax, %rsi
	movl	$.LC18, %edi
	movl	$0, %eax
	call	printf
	movl	$4, %edi
	call	malloc
	movq	%rax, -24(%rbp)
	movl	$40, %edi
	call	malloc
	movq	%rax, -32(%rbp)
	movq	-24(%rbp), %rax
	movl	$10, (%rax)
	movq	$0, -24(%rbp)
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-28.0.1)"
	.section	.note.GNU-stack,"",@progbits
