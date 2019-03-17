Since I succeeded in [porting a Leon3 inside my FPGA board](https://github.com/ttsiodras/grlib-gpl),
I decided to play a bit with SPARC assembly :-)

    $ make
    /opt/sparc-elf-3.4.4/bin/sparc-elf-gcc -mv8 -Os -o fib.elf fib.S my_printf.c -lsmall
    /opt/sparc-elf-3.4.4/bin/sparc-elf-strip fib.elf
    /opt/sparc-elf-3.4.4/bin/sparc-elf-size fib.elf
       text	   data	    bss	    dec	    hex	filename
       9744	   1684	    828	  12256	   2fe0	fib.elf

    $ /opt/grmon-eval-3.0.14/linux/bin64/grmon -u -uart /dev/ttyUSB0
    
      GRMON LEON debug monitor v3.0.14 64-bit eval version
    
      Copyright (C) 2018 Cobham Gaisler - All rights reserved.
      For latest updates, go to http://www.gaisler.com/
      Comments or bug-reports to support@gaisler.com
    
      This eval version will expire on 20/06/2019
    
      using port /dev/ttyUSB0 @ 115200 baud
      GRLIB build version: 4208
      Detected frequency:  34 MHz
    
      Component                            Vendor
      LEON3 SPARC V8 Processor             Cobham Gaisler
      AHB Debug UART                       Cobham Gaisler
      AHB/APB Bridge                       Cobham Gaisler
      LEON3 Debug Support Unit             Cobham Gaisler
      Single-port AHB SRAM module          Cobham Gaisler
      Generic UART                         Cobham Gaisler
      Multi-processor Interrupt Ctrl.      Cobham Gaisler
      Modular Timer Unit                   Cobham Gaisler
    
      Use command 'info sys' to print a detailed report of attached cores
    
    grmon3> load fib.elf
      40000000 .text                      9.5kB /   9.5kB   [===============>] 100%
      40002610 .data                      1.6kB /   1.6kB   [===============>] 100%
      Total size: 11.16kB (150.37kbit/s)
      Entry point 0x40000000
      Image /home/ttsiod/Github/SPARCasm/fib.elf loaded
    
    grmon3> run
    Computing fib(29)...
    832040
    
      Program exited normally.
    
    grmon3>
