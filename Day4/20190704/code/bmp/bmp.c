#include "bmp.h"

int fd_fb0;
int *p_init = NULL;

int init_mmap(void)
{
	p_init = mmap(NULL,800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, fd_fb0, 0);
	if(p_init == MAP_FAILED)
	{
		printf("mmap fail!\n");
		return -1;
	}	
	
	return 0;
}

int exit_mmap(void)
{
	munmap(p_init, 800*480*4);
	p_init = NULL;	
	return 0;
}

int open_fb0(void)
{
	fd_fb0 = open("/dev/fb0", O_RDWR);
	if(fd_fb0 == -1)
	{
		printf("open /dev/fb0 fail!\n");
		return -1;
	}
	else{
		printf("fd_fb0 = %d\n", fd_fb0);	
	}	
	return 0;
}

int close_fb0(void)
{
	close(fd_fb0);
	return 0;
}

int arm6818_color(int color, int coordinate_x, int coordinate_y, int height, int wide)
{
	int *p_mmap = NULL;
	p_mmap = p_init;
	
	p_mmap = p_mmap + 800* coordinate_y + coordinate_x;
		
	int x, y;
	for(y=0; y<height; y++)
	{
		for(x=0; x<wide; x++)
		{
			*(p_mmap + 800*y + x) = color;
		}
	}
	
	p_mmap = NULL;
	
	return 0;
}


int arm6818_bmp800_480(const char *pathname, int coordinate_x, int coordinate_y, int height, int wide)
{
	int i;
	int x, y;
	char A = 0x00;
	int fd_bmp;
	int *p_mmap = NULL;
	char bmp_buf[height*wide*3];
	int lcd_buf[height*wide];	
	
	fd_bmp= open(pathname, O_RDWR);
	if(fd_bmp == -1)
	{
		printf("open bmp fail!\n");
		return -1;
	}
	else{
		printf("fd_bmp = %d\n", fd_bmp);	
	}		
	
	lseek(fd_bmp, 54, SEEK_SET);
	
	bzero(bmp_buf, sizeof(bmp_buf));
	bzero(lcd_buf, sizeof(lcd_buf));
	read(fd_bmp, bmp_buf, sizeof(bmp_buf));
	close(fd_bmp);
	
	for(i=0; i<height*wide; i++)
	{
		lcd_buf[i] = bmp_buf[i*3]<<0 | bmp_buf[i*3+1]<<8 | bmp_buf[i*3+2]<<16 | A<<24;
	}
	
	p_mmap = p_init;
	p_mmap = p_mmap + 800* coordinate_y + coordinate_x;	
	for(y=0; y<height; y++)
	{
		for(x=0; x<wide; x++)
		{
			*(p_mmap + 800*(height-1-y) + x) = lcd_buf[wide*y+x];
		}
	}
	
	p_mmap = NULL;
	return 0;
}