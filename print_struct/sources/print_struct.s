	.file	"print_struct.c"

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
	subq	$400, %rsp
	testb	%al, %al
	movaps	%xmm7, -224(%rbp)
	movaps	%xmm6, -240(%rbp)
	movaps	%xmm5, -256(%rbp)
	movaps	%xmm4, -272(%rbp)
	movaps	%xmm3, -288(%rbp)
	movaps	%xmm2, -304(%rbp)
	movaps	%xmm1, -320(%rbp)
	movaps	%xmm0, -336(%rbp)
	movq	%rdi, -344(%rbp)
	movq	%r9, -352(%rbp)
	movq	%r8, -360(%rbp)
	movq	%rcx, -368(%rbp)
	movq	%rdx, -376(%rbp)
	movq	%rsi, -384(%rbp)
	je	.LBB0_5
	movaps	-336(%rbp), %xmm0
	movaps	%xmm0, -160(%rbp)
	movaps	-320(%rbp), %xmm1
	movaps	%xmm1, -144(%rbp)
	movaps	-304(%rbp), %xmm2
	movaps	%xmm2, -128(%rbp)
	movaps	-288(%rbp), %xmm3
	movaps	%xmm3, -112(%rbp)
	movaps	-272(%rbp), %xmm4
	movaps	%xmm4, -96(%rbp)
	movaps	-256(%rbp), %xmm5
	movaps	%xmm5, -80(%rbp)
	movaps	-240(%rbp), %xmm6
	movaps	%xmm6, -64(%rbp)
	movaps	-224(%rbp), %xmm7
	movaps	%xmm7, -48(%rbp)
.LBB0_5:
	movq	-352(%rbp), %rax
	movq	%rax, -168(%rbp)
	movq	-360(%rbp), %rcx
	movq	%rcx, -176(%rbp)
	movq	-368(%rbp), %rdx
	movq	%rdx, -184(%rbp)
	movq	-376(%rbp), %rsi
	movq	%rsi, -192(%rbp)
	movq	-384(%rbp), %rdi
	movq	%rdi, -200(%rbp)
	movq	-344(%rbp), %r8
	movq	%r8, -208(%rbp)
	leaq	.cst, %rdi
	movabsq	$12, %rsi
	leaq	-20(%rbp), %rax
	movl	$1, -20(%rbp)
	movl	-20(%rbp), %ecx
	movl	%ecx, -16(%rbp)
	movq	%rax, -12(%rbp)
	movb	$0, %al
	callq	printf
	movl	$1, %edi
	leaq	-16(%rbp), %rsi
	movabsq	$12, %rdx
	movl	%eax, -388(%rbp)
	movb	$0, %al
	callq	write
	movl	%eax, -392(%rbp)
	movl	$0, -24(%rbp)
	movl	-24(%rbp), %eax
	addq	$400, %rsp
	popq	%rbp
	ret
.Ltmp5:
	.size	main, .Ltmp5-main
	.cfi_endproc

	.type	.cst,@object
	.section	.rodata,"a",@progbits
	.align	8
.cst:
	.asciz	 "size of struct = %ld\n"
	.size	.cst, 22


	.section	".note.GNU-stack","",@progbits
