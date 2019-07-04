#include <stdio.h>//printf
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <unistd.h>//close
#include <linux/input.h>//struct input_event

int fd_event0;

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
	open_ts();
	
	while(1)
	{
		read_ts(&ts_x, &ts_y);
		printf("(x,y):(%d, %d)\n", ts_x, ts_y);
	}
	
	close_ts();
	return 0;
}