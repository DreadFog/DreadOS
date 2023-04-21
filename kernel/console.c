#include <n7OS/console.h>
uint8_t line = 1;
uint8_t column = 0;
uint8_t *scr_tab = (uint8_t *)0xB8000;
int pos;
int font_color = 0xF;
int bg_color = 0x0;
void scroll()
{
    for (int i = 80; i < 80 * 24; i++)
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
/* Function to put one character at the pointer position*/
void console_putchar(const char c)
{
    pos = (80 * line + column) * 2;
    if (c > 31 && c < 127)
    {
        scr_tab[pos] = c;
        scr_tab[pos + 1] = font_color | (bg_color << 4);
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
/* Function to write bytes at the current pointer position*/
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
    set_layout();
}
/* Function to write in a specific position of the terminal*/
void console_putbytes_position(const char *s, int len, int l, int c){
    uint8_t copy_l = line;
    uint8_t copy_c = column;
    line =l;
    column = c;
    console_putbytes(s, len);
    line = copy_l;
    column = copy_c;
    pointer();
}
/* Function to write in the first line of the terminal*/
void console_putbytes_reserved(const char *s, int len, int c){
    console_putbytes_position(s, len, 0, c);
}
void console_print_time(int h, int m, int s) {
    uint8_t copy_l = line;
    uint8_t copy_c = column;
    uint8_t copy_bg = bg_color;
    uint8_t copy_font = font_color;
    font_color = RED;
    bg_color = BLACK;
    line = 0;
    column = 62;
    printf("Uptime: %d:%2d:%2d\n", h, m, s);
    line = copy_l;
    column = copy_c;
    bg_color = copy_bg;
    font_color = copy_font;
    pointer();
}
void change_color(uint8_t bg, uint8_t font) {
    bg_color = bg;
    font_color = font;
}
void set_layout() {
    change_color(BLACK, MAGENTA);
    console_putbytes_reserved("DreadOS, by DreadFog (Quentin FRATY)", 36, 0);
    change_color(BLACK, WHITE);
}

