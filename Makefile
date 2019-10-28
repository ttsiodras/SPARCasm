all:	fib.elf

fib.elf:	fib.S my_printf.c
	/opt/sparc-elf-3.4.4/bin/sparc-elf-gcc -mv8 -Os -o $@ $^ -lsmall
	/opt/sparc-elf-3.4.4/bin/sparc-elf-strip $@
	/opt/sparc-elf-3.4.4/bin/sparc-elf-size $@

simulate:	fib.elf
	qemu-system-sparc -M leon3_generic -nographic -kernel $<

clean:
	rm -f fib.elf
