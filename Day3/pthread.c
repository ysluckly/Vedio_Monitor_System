#include <stdio.h>//printf
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <unistd.h>//close、sleep
#include <sys/mman.h>//mmap、munmap
#include<pthread.h>

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

void *task1_ser(void *arg)
{
	while(1)
	{
		arm6818_color(0x000000ff, 0, 0, 240, 400);//BGR
		sleep(0.01);	
		arm6818_color(0x00ff0000, 0, 0, 240, 400);//BGR
		sleep(0.01);
		arm6818_color(0x0000ff00, 0, 0, 240, 400);//BGR
		sleep(0.01);
	
	}	
	pthread_exit(NULL);
}

void *task2_ser(void *arg)
{
	while(1)
	{
		arm6818_color(0x000000ff, 400, 0, 240, 400);//BGR
		sleep(0.1);	
		arm6818_color(0x00ff0000, 400, 0, 240, 400);//BGR
		sleep(0.1);
		arm6818_color(0x0000ff00, 400, 0, 240, 400);//BGR
		sleep(0.1);
	
	}	
		pthread_exit(NULL);
}
void *task3_ser(void *arg)
{
	while(1)
	{
		arm6818_color(0x000000ff, 0,240, 240, 400);//BGR
		sleep(1);	
		arm6818_color(0x00ff0000,0,240, 240, 400);//BGR
		sleep(1);
		arm6818_color(0x0000ff00, 0,240, 240, 400);//BGR
		sleep(1);
	
	}	
		pthread_exit(NULL);
}
void *task4_ser(void *arg)
{
	while(1)
	{
		arm6818_color(0x000000ff, 400, 240, 240, 400);//BGR
		sleep(2);	
		arm6818_color(0x00ff0000, 400, 240,240, 400);//BGR
		sleep(2);
		arm6818_color(0x0000ff00, 400, 240, 240, 400);//BGR
		sleep(2);
	
	}	
		pthread_exit(NULL);
}
int main(void)//主线程函数
{
	open_fb0();
	init_mmap();
	
	pthread_t task1_id;
	pthread_t task2_id;
	pthread_t task3_id;
	pthread_t task4_id;

	pthread_create(&task1_id, NULL, task1_ser, NULL);
	pthread_create(&task2_id, NULL, task2_ser, NULL);
	pthread_create(&task3_id, NULL, task3_ser, NULL);
	pthread_create(&task4_id, NULL, task4_ser, NULL);
		
	
	pthread_join(task1_id,NULL);
	pthread_join(task2_id,NULL);
	pthread_join(task3_id,NULL);
	pthread_join(task4_id,NULL);
	
	exit_mmap();
	close_fb0();
	return 0;
}
