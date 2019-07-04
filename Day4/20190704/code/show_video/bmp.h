#ifndef _BMP_H
#define _BMP_H

#include <stdio.h>//printf
#include <sys/types.h>//open,lseek
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <strings.h>//bzero
#include <unistd.h>//lseek,read
#include <sys/mman.h>//mmap,munmap

extern int show_bmp800_480(char *pathname);

#endif