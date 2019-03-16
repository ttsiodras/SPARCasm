	.section ".text"
	.global main

	.align 4
main: 	save	%sp,-120,%sp
	set	string, %o0
	call	printf
	nop
	set	format, %o0
 	set	inpVal, %o1
	or	%g0,%o1,%l1
	call	scanf
	nop
 	ld	[%l1 + %g0], %o0
	call	fib
	nop
	or	%o0,%g0, %o1
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
	or	%g0, %o0, %l0

.ifdef DEBUG
	or	%g0, %o0, %o2
	or	%g0, %i0, %o1
	set     dbg1, %o0
	call	printf
	nop
.endif

	sub	%i0,2,%o0
	call	fib
	nop
	or	%g0, %o0, %l2

.ifdef DEBUG
	or	%g0, %o0, %o1
	set     formatn, %o0
	call	printf
	nop
.endif
	add	%l0, %l2, %i0

	ret
	restore

	.section ".data"
string: .asciz   "Enter input number:\n> "
	.align 4
format: .asciz   "%d"
	.align 4
formatn:.asciz  "%d\n"
	.align 4
dbg1:   .asciz   "fib(%d) = %d + "
	.align 4
inpVal:	.word