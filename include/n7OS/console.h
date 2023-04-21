#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include <n7OS/cpu.h>
#include <inttypes.h>
#include <stdio.h>
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
void console_print_time(int h, int m, int s);
// Colors
#define BLACK   0
#define BLUE    1
#define GREEN   2
#define CYAN    3
#define RED     4
#define MAGENTA 5
#define BROWN   6
#define GRAY   7
#define DGRAY    8
#define LBLUE   9
#define LGREEN  10
#define LCYAN   11
#define LRED    12
#define LMAGENTA 13
#define YELLOW  14
#define WHITE   15

void change_color(uint8_t bg, uint8_t font);
void set_layout();
#endif
