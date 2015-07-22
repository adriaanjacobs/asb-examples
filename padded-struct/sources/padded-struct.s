	.file	"padded-struct.c"

	.ident	"GCC: (Ubuntu/Linaro 4.7.3-12ubuntu1) 4.7.3 LLVM: 3.3"


	.text
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:
	.cfi_startproc
	pushq	%rbp
.Ltmp2:
	.cfi_def_cfa_offset 16
.Ltmp3:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
.Ltmp4:
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movl	%edi, -4(%rbp)
	movq	%rsi, -16(%rbp)
	leaq	.cst, %rdi
	movabsq	$12, %rsi
	movb	$97, -32(%rbp)
	movl	$0, -28(%rbp)
	movl	$1, -24(%rbp)
	movb	$0, %al
	callq	printf
	movl	$2, %edi
	leaq	-32(%rbp), %rsi
	movabsq	$12, %rdx
	movl	%eax, -40(%rbp)
	movb	$0, %al
	callq	write
	movl	%eax, -44(%rbp)
	movl	$0, -36(%rbp)
	movl	-36(%rbp), %eax
	addq	$48, %rsp
	popq	%rbp
	ret
.Ltmp5:
	.size	main, .Ltmp5-main
	.cfi_endproc

	.type	.cst,@object
	.section	.rodata,"a",@progbits
	.align	8
.cst:
	.asciz	 "sizeof padded_struct is %ld\n"
	.size	.cst, 29


	.section	".note.GNU-stack","",@progbits
