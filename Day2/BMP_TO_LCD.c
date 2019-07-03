/*************************************************************************
	> File Name:    BMP_TO_LED.c
	> Author:       Hunter
	> Mail:         hunter.520@qq.com
	> Created Time: 2019年07月02日 星期二 00时34分08秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main()
{
	//1）打开LCD屏幕，打开图片。
	int fd_bmp = open("/root/project1/images/3.bmp",O_RDWR);
	int fd_lcd = open("/dev/fb0",O_RDWR);
	if(fd_bmp < 0 || fd_lcd <0)
	{
        printf("BMP Filescip iS %d AND LCD Filescip is %d \n",fd_bmp,fd_lcd);
        return -1;
    }

    printf("BMP Filescip iS %d AND LCD Filescip is %d \n",fd_bmp,fd_lcd);
	int ret = lseek(fd_bmp, 54, SEEK_SET);
	if (ret < 0)
	{
		printf("lseek error %d\n",ret);
		return -2;
	}
	
	//2）创建映射通道，将内存中图片的颜色数据通过p_mmap放到LCD屏幕。
	int *p_init = NULL;
	int *p_mmap = NULL;
	
	p_init = mmap(NULL,800 * 480 * 4,PROT_READ | PROT_WRITE,MAP_SHARED,fd_lcd,0);
	p_mmap = p_init;

	//2-1）800*480个BMP像素点变成800*480个LCD像素点
	
    char buf_bmp[800*480*3];
    int  buf_lcd[800*480];
	bzero(buf_bmp, sizeof(buf_bmp));
	bzero(buf_lcd, sizeof(buf_lcd));
	
	ret = read(fd_bmp, buf_bmp, sizeof(buf_bmp));
	if (ret < 0)
	{
		printf("read error\n");
		return -3;
	}
	
	char A = 0x00;
	int i = 0;
	for (i = 0; i<800 * 480; i++)
	{
		buf_lcd[i] = buf_bmp[i * 3] << 0 | buf_bmp[i * 3 + 1] << 8 | buf_bmp[i * 3 + 2] << 16 | A << 24;
	}
	
	//2-2）颠倒图片
	int xx = 0;
	int y = 0;
	for (xx = 479; xx >= 0; xx--)
	{
		for (y = 0; y<800; y++)
		{
			*p_mmap= buf_lcd[xx*800+y];
			p_mmap++;
			 
		}
	}

	//3）关闭LCD屏幕，关闭图片，释放映射空间的资源。
   
	munmap(p_init, 800*480*4); 
	close(fd_bmp);
	close(fd_lcd);
	p_init = NULL;
	p_mmap=NULL;
    return 0;
}
