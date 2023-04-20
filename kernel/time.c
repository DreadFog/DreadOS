#include <n7OS/time.h>
extern void handler_IT_timer();

uint32_t curr_time = 0;
uint32_t f_osc = 0x1234BD; // 1.19 MHz
uint32_t f = 1000;
uint8_t t[3] = {0, 0, 0}; // [s, mins, h]
int millis_per_sec;
int millis_per_min;
int millis_per_hour;
int millis_per_day;
void init_timer(uint32_t clock_freq)
{
    millis_per_sec = clock_freq;
    millis_per_min = 60 * millis_per_sec;
    millis_per_hour = 60 * millis_per_min;
    curr_time = 0;
    t[0] = 0;
    t[1] = 0;
    t[2] = 0;
    f = f_osc / clock_freq;
    // PIT configuration
    outb(0x34, 0x43);
    // freq saving
    outb(f & 0xFF, 0x40);
    outb(f >> 8, 0x40);
    // activating the timer IT
    outb(inb(0x21) & 0xfe, 0x21);
    init_irq_entry(0x20, (uint32_t)handler_IT_timer);
}
void update_time()
{
    t[2] = curr_time / millis_per_hour;
    t[1] = (curr_time % millis_per_hour) / millis_per_min;
    t[0] = (curr_time % millis_per_min) / millis_per_sec;
    printf("%d:%d:%d\n", t[2], t[1], t[0]);
}
void handler_timer()
{
    // ack IT
    outb(0x20, 0x20);
    // inc time
    // printf("inc");
    curr_time++;
    update_time();
}
