	.file	"basics.c"
	.text
	.globl	doException
	.type	doException, @function
doException:
.LFB22:
	.cfi_startproc
	subq	$40, %rsp
	.cfi_def_cfa_offset 48
	movabsq	$8028075836850796645, %rax
	movabsq	$7959953403570036846, %rdx
	movq	%rax, (%rsp)
	movq	%rdx, 8(%rsp)
	movw	$10, 16(%rsp)
	movl	$17, %edx
	movq	%rsp, %rsi
	movl	$1, %edi
	call	write
	movl	$1, %edi
	call	_exit
	.cfi_endproc
.LFE22:
	.size	doException, .-doException
	.section	.rodata.str1.1,"aMS",@progbits,1
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
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC7:
	.string	"The highest possible integer (using macro MAX_INT):"
	.section	.rodata.str1.1
.LC8:
	.string	"%d\n"
	.section	.rodata.str1.8
	.align 8
.LC9:
	.string	"The lowest possible integer is: %d\n"
	.align 8
.LC10:
	.string	"INT_MAX + 1 = %d (overflows to negative due to leading 1)\n"
	.align 8
.LC11:
	.string	"UINT_MAX + 1 = %d (ignores leading 1, rest of bits are 0)\n"
	.section	.rodata.str1.1
.LC12:
	.string	"Enter an integer: "
	.section	.rodata.str1.8
	.align 8
.LC13:
	.string	"Invalid input. Enter an integer: "
	.section	.rodata.str1.1
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
	.section	.rodata.str1.8
	.align 8
.LC19:
	.string	"hello from the child, my pid is: %d and my parents is %d\n"
	.align 8
.LC20:
	.string	"hello from the parent, my pid is %d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB23:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	subq	$16, %rsp
	.cfi_def_cfa_offset 32
	movslq	MAX_LENGTH(%rip), %rdi
	call	malloc
	movq	%rax, %rbx
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
.L4:
	leaq	12(%rsp), %rsi
	movl	$.LC14, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	testl	%eax, %eax
	jne	.L9
	movq	stdin(%rip), %rdi
	call	getc
	movl	$.LC13, %edi
	movl	$0, %eax
	call	printf
	jmp	.L4
.L9:
	movl	12(%rsp), %esi
	movl	$.LC15, %edi
	movl	$0, %eax
	call	printf
	movq	stdin(%rip), %rdi
	call	getc
	movl	$.LC16, %edi
	movl	$0, %eax
	call	printf
	movq	%rbx, %rsi
	movl	$.LC17, %edi
	movl	$0, %eax
	call	__isoc99_scanf
	movq	stdin(%rip), %rdi
	call	getc
	movq	%rbx, %rsi
	movl	$.LC18, %edi
	movl	$0, %eax
	call	printf
	movl	$4, %edi
	call	malloc
	movl	$10, (%rax)
	call	fork
	testl	%eax, %eax
	jne	.L6
	call	getppid
	movl	%eax, %ebx
	call	getpid
	movl	%ebx, %edx
	movl	%eax, %esi
	movl	$.LC19, %edi
	movl	$0, %eax
	call	printf
.L7:
	movl	$doException, %esi
	movl	$8, %edi
	call	signal
	movl	$0, %eax
	addq	$16, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 16
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
.L6:
	.cfi_restore_state
	call	getpid
	movl	%eax, %esi
	movl	$.LC20, %edi
	movl	$0, %eax
	call	printf
	jmp	.L7
	.cfi_endproc
.LFE23:
	.size	main, .-main
	.globl	MAX_LENGTH
	.data
	.align 4
	.type	MAX_LENGTH, @object
	.size	MAX_LENGTH, 4
MAX_LENGTH:
	.long	100
	.ident	"GCC: (GNU) 8.5.0 20210514 (Red Hat 8.5.0-28.0.1)"
	.section	.note.GNU-stack,"",@progbits
