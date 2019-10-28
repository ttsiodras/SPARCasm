all:	fib.elf

fib.elf:	fib.S my_printf.c
	/opt/sparc-elf-3.4.4/bin/sparc-elf-gcc -mv8 -Os -o $@ $^ -lsmall
	/opt/sparc-elf-3.4.4/bin/sparc-elf-strip $@
	/opt/sparc-elf-3.4.4/bin/sparc-elf-size $@

simulate:	fib.elf
	qemu-system-sparc -M leon3_generic -nographic -kernel $<

runInFPGA:	fib.elf
	bash -c '/opt/grmon-eval-3.1.0/linux/bin64/grmon -u -xilusb -c <(echo -e "load fib.elf\nrun\nquit\n")'

clean:
	rm -f fib.elf
