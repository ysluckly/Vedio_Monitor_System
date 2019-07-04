#include "touch_screen.h"

int fd_ev0;

int open_ts(void)//1，打开触控屏(open)
{	
	fd_ev0 = open("/dev/input/event0", O_RDWR);
	if(fd_ev0 == -1)
	{
			printf("open event0 fail!\n");
	}		
	return 0;
}

int read_ts(int *coordinate_x, int *coordinate_y)//2，读取触控屏的信息到结构体中。(read)
{
	int ret;	
	struct input_event coordinate;
	while(1)
	{
		ret = read(fd_ev0, &coordinate, sizeof(struct input_event));
		if(ret == -1)
		{
			printf("read bmp fail!\n");
			return -1;		
		}			
		//3，打印结构体中获取的触控屏信息。(printf)
		//printf("type: %d, code: %d, value:%d\n",coordinate.type, coordinate.code, coordinate.value);
		/*
			type: 3, code: 0, value:344//X轴的坐标
			type: 3, code: 1, value:113//Y轴的坐标
			type: 1, code: 330, value:0		
		*/
		if(coordinate.type==3 && coordinate.code==0 && coordinate.value>0 && coordinate.value<800)
		{
			*coordinate_x =  coordinate.value;
		}
		if(coordinate.type==3 && coordinate.code==1 && coordinate.value>0 && coordinate.value<480)
		{
			*coordinate_y =  coordinate.value;
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
	close(fd_ev0);
	return 0;
}