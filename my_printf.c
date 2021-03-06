// A custom printf made specifically for my ZestSC1 experiments.
//
// It is based on a BSD-licensed implementation of printf, supporting only %d,
// %u, %x, %X, %c and %s.  It is offered without guarantees of correctness - it
// works fine in my local tests, and since serial output is only meant to be
// used during debugging, this is anyway code that is not supposed to "fly".
//
// Original code available in: https://github.com/mpredfearn/simple-printf
// (BSD Licensed).
//
// Using this instead of normal printf (which is thankfully defined as a weak
// symbol) drops my .text usage dramatically - enough to allow me to run
// in my tiny 16KB of BlockRAM.

#include "my_printf.h"

#define DREADY 1
#define TREADY 4

void leon_putc(unsigned char c)
{
    volatile int *console = (volatile int *) 0x80000100;

    volatile int *rxstat;
    volatile int *rxadata;
    int rxmask;
    while ((console[1] & TREADY) == 0);
    console[0] = (0x0ff & c);
    if (c == '\n') {
        while ((console[1] & TREADY) == 0);
        console[0] = (int) '\r';
        while ((console[1] & TREADY) == 0);
    }
}

enum flags {
    PAD_ZERO	= 1,
    PAD_RIGHT	= 2,
};

static int prints(const char *string, int width, int flags)
{
    int pc = 0, padchar = ' ';

    if (width > 0) {
        int len = 0;
        const char *ptr;
        for (ptr = string; *ptr; ++ptr) ++len;
        if (len >= width) width = 0;
        else width -= len;
        if (flags & PAD_ZERO)
            padchar = '0';
    }
    if (!(flags & PAD_RIGHT)) {
        for ( ; width > 0; --width) {
            leon_putc(padchar);
            ++pc;
        }
    }
    for ( ; *string ; ++string) {
        leon_putc(*string);
        ++pc;
    }
    for ( ; width > 0; --width) {
        leon_putc(padchar);
        ++pc;
    }

    return pc;
}

#define PRINT_BUF_LEN 64

static int simple_outputi(
    int i, int base, int sign, int width, int flags, int letbase)
{
    char print_buf[PRINT_BUF_LEN];
    char *s;
    int t, neg = 0, pc = 0;
    unsigned int u = i;

    if (i == 0) {
        print_buf[0] = '0';
        print_buf[1] = '\0';
        return prints(print_buf, width, flags);
    }

    if (sign && base == 10 && i < 0) {
        neg = 1;
        u = -i;
    }

    s = print_buf + PRINT_BUF_LEN-1;
    *s = '\0';

    while (u) {
        t = u % base;
        if( t >= 10 )
            t += letbase - '0' - 10;
        *--s = t + '0';
        u /= base;
    }

    if (neg) {
        if( width && (flags & PAD_ZERO) ) {
            leon_putc ('-');
            ++pc;
            --width;
        }
        else {
            *--s = '-';
        }
    }

    return pc + prints (s, width, flags);
}

static int simple_vsprintf(const char *format, va_list ap)
{
    int width, flags;
    int pc = 0;
    char scr[2];
    union {
        char c;
        char *s;
        int i;
        unsigned int u;
        void *p;
    } u;

    for (; *format != 0; ++format) {
        if (*format == '%') {
            ++format;
            width = flags = 0;
            if (*format == '\0')
                break;
            if (*format == '%')
                goto out;
            if (*format == '-') {
                ++format;
                flags = PAD_RIGHT;
            }
            while (*format == '0') {
                ++format;
                flags |= PAD_ZERO;
            }
            if (*format == '*') {
                width = va_arg(ap, int);
                format++;
            } else {
                for ( ; *format >= '0' && *format <= '9'; ++format) {
                    width *= 10;
                    width += *format - '0';
                }
            }
            switch (*format) {
                case('d'):
                    u.i = va_arg(ap, int);
                    pc += simple_outputi(u.i, 10, 1, width, flags, 'a');
                    break;
#ifndef SMALL_ZESTSC1
                case('u'):
                    u.u = va_arg(ap, unsigned int);
                    pc += simple_outputi(u.u, 10, 0, width, flags, 'a');
                    break;

                case('x'):
                    u.u = va_arg(ap, unsigned int);
                    pc += simple_outputi(u.u, 16, 0, width, flags, 'a');
                    break;

                case('X'):
                    u.u = va_arg(ap, unsigned int);
                    pc += simple_outputi(u.u, 16, 0, width, flags, 'A');
                    break;

                case('c'):
                    u.c = va_arg(ap, int);
                    scr[0] = u.c;
                    scr[1] = '\0';
                    pc += prints(scr, width, flags);
                    break;

                case('s'):
                    u.s = va_arg(ap, char *);
                    pc += prints(u.s ? u.s : "(null)", width, flags);
                    break;
#endif
                default:
                    break;
            }
        }
        else {
out:
            leon_putc (*format);
            ++pc;
        }
    }
    return pc;
}

int printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int ret = simple_vsprintf(fmt, ap);
    va_end(ap);
    return ret;
}
