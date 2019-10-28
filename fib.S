	.section ".text"
	.global main

	.align 4
main: 	save	%sp,-120,%sp
	set	29, %l5
	mov     %l5,%o1
	set	string, %o0
	call	printf
	nop
 	set	inpVal, %o1
	mov     %o1,%l1
 	st	%l5, [%l1]
 	ld	[%l1], %o0
	call	fib
	nop
	mov     %o0, %o1
	set	formatn, %o0
	call	printf
	nop
	ret
	restore

	.align	4
fib:	save	%sp,-120,%sp
	cmp	%i0, 1
	bg	recur
	nop
	add	%g0,1,%i0
	ret
	restore

	.align 4
recur:	sub	%i0,1,%o0
	call	fib
	nop
	mov     %o0, %l0

#ifdef DEBUG
	mov     %o0, %o2
	mov     %i0, %o1
	set     dbg1, %o0
	call	printf
	nop
#endif

	sub	%i0,2,%o0
	call	fib
	nop
	mov     %o0, %l2

#ifdef DEBUG
	mov     %o0, %o1
	set     formatn, %o0
	call	printf
	nop
#endif
	add	%l0, %l2, %i0
	ret
	restore

	.section ".data"
string: .asciz   "Computing fib(%d)...\n"
	.align 4
format: .asciz   "%d"
	.align 4
formatn:.asciz  "%d\n"
	.align 4
dbg1:   .asciz   "fib(%d) = %d + "
	.align 4
inpVal:	.word
