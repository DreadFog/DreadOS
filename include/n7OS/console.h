#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#define CONSOLE_WIDTH 25
#define CONSOLE_LENGTH 80
#define CONSOLE_SIZE 2000
/*
 * This is the function called by printf to send its output to the screen. You
 * have to implement it in the kernel and in the user program.
 */
void console_putbytes(const char *s, int len);

void console_putbytes_position(const char *s, int len, int l, int c);
void clear_console();
#endif
