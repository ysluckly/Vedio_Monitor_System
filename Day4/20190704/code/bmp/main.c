#include <stdio.h>//printf
#include "bmp.h"

int main(void)
{
	open_fb0();
	init_mmap();
	
	arm6818_color(0x00ff0000, 200, 200, 10,10);
	arm6818_bmp800_480("/root/project/images/0.bmp", 10, 10, 40, 40);
	
	exit_mmap();
	close_fb0();
	return 0;
}