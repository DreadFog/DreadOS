#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <inttypes.h>

uint8_t line = 0;
uint8_t column = 0;
uint8_t *scr_tab = (uint8_t *)0xB8000;
int pos;
void scroll()
{
    for (int i = 0; i < 80 * 24; i++)
    {
        scr_tab[i * 2] = scr_tab[(i + 80) * 2];
        scr_tab[i * 2 + 1] = scr_tab[(i + 80) * 2 + 1];
    }
    for (int i = 80 * 24; i < 80 * 25; i++)
    {
        scr_tab[i * 2] = ' ';
        scr_tab[i * 2 + 1] = 0x0F;
    }
    line = 24;
}

void pointer()
{

    uint16_t pos = (80 * line + column);
    uint8_t low = pos & 0xFF;
    uint8_t high = (pos >> 8) & 0xFF;
    outb(0xF, 0x3D4);
    outb(low, 0x3D5);
    outb(0xE, 0x3D4);
    outb(high, 0x3D5);
}
void console_putchar(const char c)
{
    pos = (80 * line + column) * 2;
    if (c > 31 && c < 127)
    {
        scr_tab[pos] = c;
        scr_tab[pos + 1] = 0x0F; // white on black
        column++;
    }
    else if (c == '\n')
    {
        scr_tab[pos] = ' ';
        column = 0;
        line++;
    }
    else if (c == '\t')
    { // tab
        column += 8;
    }
    else if (c == '\b')
    { // backspace
        if (column > 0)
        {
            column--;
        }
    }
    else if (c == '\f')
    { // form feed
        for (int i = 0; i < 80 * 25; i++)
        {
            scr_tab[i * 2] = ' ';
            scr_tab[i * 2 + 1] = 0x0F;
        }
    }
    else if (c == '\r')
    { // carriage return
        column = 0;
    }
    else if (c == '\v')
    { // vertical tab
        line++;
    }
    if (column >= 80)
    {
        column = 0;
        line++;
    }
    if (line == CONSOLE_WIDTH)
    {
        scroll();
    }
}
void console_putbytes(const char *s, int len)
{
    for (int i = 0; i < len; i++)
    {
        console_putchar(s[i]);
    }
    pointer();
}
void clear_console()
{
    console_putchar('\f');
}
void console_putbytes_position(const char *s, int len, int l, int c){
    line = l;
    column = c;
    console_putbytes(s, len);
}
