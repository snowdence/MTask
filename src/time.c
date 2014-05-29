#include "kernel.h"
#include "../include/time.h"

/* Fuente consultada: http://wiki.osdev.org/RTC */

const char *_days[] = {
  "Sunday", "Monday", "Tuesday", "Wednesday",
  "Thursday", "Friday", "Saturday"
};

const char *_days_abbrev[] = {
  "Sun", "Mon", "Tue", "Wed", 
  "Thu", "Fri", "Sat"
};

const char *_months[] = {
  "January", "February", "March",
  "April", "May", "June",
  "July", "August", "September",
  "October", "November", "December"
};

const char *_months_abbrev[] = {
  "Jan", "Feb", "Mar",
  "Apr", "May", "Jun",
  "Jul", "Aug", "Sep",
  "Oct", "Nov", "Dec"
};


void time(time_t * tp) {
    Atomic();
    outb(0x70, 4);
    tp->tm_hour = inb(0x71);
    outb(0x70, 2);
    tp->tm_min = inb(0x71);
    outb(0x70, 0);
    tp->tm_sec = inb(0x71);
    outb(0x70, 6);
    tp->tm_wday = inb(0x71);
    outb(0x70, 7);
    tp->tm_mday = inb(0x71);
    outb(0x70, 8);
    tp->tm_mon = inb(0x71);
    outb(0x70, 9);
    tp->tm_year = inb(0x71);
    Unatomic();
    return;
}

char * asctime(char * str_time, const time_t * tp) {
    // Thu May 29 11:35:33 2014
    char time[9];
    int wday, mon;
    char day[3];
    char year[3];
    wday = ((tp->tm_wday & 0xF0) >> 4) * 10 + (tp->tm_wday & 0x0F);
    strcpy(str_time, _days_abbrev[wday]);
    strcat(str_time, " ");
    mon = ((tp->tm_mon & 0xF0) >> 4) * 10 + (tp->tm_mon & 0x0F);
    strcat(str_time, _months_abbrev[mon]);
    strcat(str_time, " ");
    day[0] = ((tp->tm_mday & 0xF0) >> 4) + '0';
    day[1] = ((tp->tm_mday & 0x0F)) + '0';
    day[2] = 0;
    strcat(str_time, day);
    strcat(str_time, " ");
    time[0] = ((tp->tm_hour & 0xF0) >> 4) + '0';
    time[1] = ((tp->tm_hour & 0x0F)) + '0';
    time[2] = ':';
    time[3] = ((tp->tm_min & 0xF0) >> 4) + '0';
    time[4] = ((tp->tm_min & 0x0F)) + '0';
    time[5] = ':';
    time[6] = ((tp->tm_sec & 0xF0) >> 4) + '0';
    time[7] = ((tp->tm_sec & 0x0F)) + '0';
    time[8] = 0;
    strcat(str_time, time);
    strcat(str_time, " ");
    year[0] = ((tp->tm_year & 0xF0) >> 4) + '0';
    year[1] = ((tp->tm_year & 0x0F)) + '0';
    year[2] = 0;
    strcat(str_time, "20");
    strcat(str_time, year);
    return str_time;
}

int time_main(int argc, char * argv[]) {
    char timeString[24];
    time_t t;
    time(&t);
    printk("%s.\n", asctime(timeString, &t));
    return 0;
}

