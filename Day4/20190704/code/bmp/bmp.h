#ifndef _BMP_H
#define _BMP_H

#include <stdio.h>//printf
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <unistd.h>//close
#include <sys/mman.h>//mmap¡¢munmap
#include <strings.h>//bzero

extern int init_mmap(void);
extern int exit_mmap(void);
extern int open_fb0(void);
extern int close_fb0(void);
extern int arm6818_bmp800_480(const char *pathname, int coordinate_x, int coordinate_y, int height, int wide);
extern int arm6818_color(int color, int coordinate_x, int coordinate_y, int height, int wide);

#endif