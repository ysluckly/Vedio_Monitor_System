#ifndef __TOUCH_SCREEN_H
#define __TOUCH_SCREEN_H

#include <stdio.h>//printf
#include <linux/input.h>//struct input_event
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <unistd.h>//read

extern int open_ts(void);
extern int read_ts(int *coordinate_x, int *coordinate_y);
extern int close_ts(void);

#endif