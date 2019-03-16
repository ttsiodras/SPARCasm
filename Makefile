all:	fib.elf

fib.elf:	fib.S
	/opt/sparc-elf-3.4.4/bin/sparc-elf-gcc -g -o $@ $<

clean:
	rm -f fib.elf

