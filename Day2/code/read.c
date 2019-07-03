#include <stdio.h>//printf
#include <sys/types.h>//open
#include <sys/stat.h>//open
#include <fcntl.h>//open
#include <unistd.h>//close

int main(void)
{
	int fd_txt;
	fd_txt = open("/home/gec/Desktop/1.txt", O_RDWR);
	if(fd_txt == -1)
	{
		printf("open txt fail!\n");
		return -1;
	}
	else{
		printf("fd_txt = %d\n", fd_txt);	
	}
	
	
	
	
	
	close(fd_txt);
	return 0;
}