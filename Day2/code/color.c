#include <stdio.h>//printf
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <unistd.h>//close
#include <sys/mman.h>//mmap、munmap

int main(void)
{
	int fd_fb0;
	fd_fb0 = open("/dev/fb0", O_RDWR);
	if(fd_fb0 == -1)
	{
		printf("open /dev/fb0 fail!\n");
		return -1;
	}
	else{
		printf("fd_fb0 = %d\n", fd_fb0);	
	}
	
	int color = 0x0000ff00;
	int *p_mmap = NULL;
	int *p_init = NULL;
	
	p_init = mmap(NULL,800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, fd_fb0, 0);
	if(p_init == MAP_FAILED)
	{
		printf("mmap fail!\n");
		return -1;
	}
	
	p_mmap = p_init;
	*p_mmap = color;	
	
	close(fd_fb0);
	munmap(p_init, 800*480*4);
	p_mmap = NULL;
	p_init = NULL;
	
	return 0;
}