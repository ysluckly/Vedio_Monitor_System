/*************************************************************************
	> File Name:    BMP_TO_LED.c
	> Author:       Hunter
	> Mail:         hunter.520@qq.com
	> Created Time: 2019年07月02日 星期二 00时34分08秒
 ************************************************************************/

#include <stdio.h>//printf
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <unistd.h>//close
#include <linux/input.h>//struct input_event
#include <sys/mman.h>

int fd_event0;
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
int open_ts(void)
{

	fd_event0 = open("/dev/input/event0", O_RDWR);
	if(fd_event0 == -1)
	{
		printf("open event0 fail!\n");
		return -1;
	}
	else{
		printf("fd_event0 = %d\n", fd_event0);	
	}	
	return 0;
}

int read_ts(int *coordinate_x, int *coordinate_y)
{
	struct input_event coordinate;
	while(1)
	{
		read(fd_event0, &coordinate, sizeof(struct input_event));
		//printf("type:%d; code:%d; value:%d\n", coordinate.type, coordinate.code, coordinate.value);
		/*
			type:3; code:0; value:457//X轴坐标轴
			type:3; code:1; value:349//Y轴坐标轴
			type:3; code:24; value:200
			type:1; code:330; value:1
			type:0; code:0; value:0
			type:3; code:24; value:0
			type:1; code:330; value:0
			type:0; code:0; value:0				
		*/
		if(coordinate.type==3 && coordinate.code==0 && coordinate.value>=0 && coordinate.value<800)
		{
			*coordinate_x = coordinate.value;
		}
		if(coordinate.type==3 && coordinate.code==1 && coordinate.value>=0 && coordinate.value<480)
		{
			*coordinate_y = coordinate.value;
		}		
		if(coordinate.type==1 && coordinate.code==330 && coordinate.value==0)
		{
			break;
		}				
	}		
	
	
	
	return 0;
}

int close_ts(void)
{
	close(fd_event0);
	return 0;
}

int main(void)
{
	int ts_x, ts_y;
	int red=-1;
	int green = -1;
	int blue = -1;
	
	open_ts();

	open_fb0();
	init_mmap();
	
	while(1)
	{
		read_ts(&ts_x, &ts_y);
		printf("The X/Y is: (x,y)--->(%d, %d)\n", ts_x, ts_y);
		if(ts_x>0&&ts_x<200&&ts_y>0&&ts_y<480)
		{
			red = -red;
			if(red>0)
				arm6818_color(0x00ff0000, 0,0 , 480, 200);	
			else
				arm6818_color(0xffffffff,0,0,480,200);
		}
		else if(ts_x>200&&ts_x<600&&ts_y>0&&ts_y<480)
		{
			green = -green;
			if(green>0)
				arm6818_color(0x0000ff00, 200,0 , 480, 400);	
			else
				arm6818_color(0xffffffff,200,0,480,400);
		}
		else if(ts_x>600&&ts_x<800&&ts_y>0&&ts_y<480)
		{
			blue = -blue;
			if(blue>0)
				arm6818_color(0x000000ff, 600,0 , 480, 200);	
			else
				arm6818_color(0xffffffff,600,0,480,200);
		}
	}
	
	close_ts();
	close_fb0();
	exit_mmap();	
	return 0;
}
