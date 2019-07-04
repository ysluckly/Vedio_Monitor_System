#include "bmp.h"

int show_bmp800_480(char *pathname)
{
	int i;
	int ret;
	int x, y;
	int fd_bmp, fd_lcd;
	//1，打开图片，打开LCD屏幕。（open）	
	fd_bmp = open(pathname, O_RDWR);
	fd_lcd = open("/dev/fb0", O_RDWR);	
	if(fd_bmp==-1 || fd_lcd==-1)
	{
		printf("open bmp or lcd fail!\n");
		return -1;
	}
	
	//2，去掉不是像素的信息（54byte）。（lseek）
	ret = lseek(fd_bmp, 54, SEEK_SET);
	if(ret == -1)
	{
		printf("lseek bmp fail!\n");
	}
	
	//3，读取BMP图片的像素到LCD屏幕中。（read，mmap）	
	char bmp_buf[800*480*3];
	int lcd_buf[800*480];
	bzero(bmp_buf, sizeof(bmp_buf));
	bzero(lcd_buf, sizeof(lcd_buf));
	
	ret = read(fd_bmp, bmp_buf, sizeof(bmp_buf));
	if(ret == -1)
	{
		printf("read bmp fail!\n");
		return -1;		
	}
	
	//算法1：800*480BMP像素变成800*480LCD像素。
	for(i=0; i<800*480; i++)
	{
		lcd_buf[i] = bmp_buf[i*3]<<0 | bmp_buf[i*3+1]<<8 | bmp_buf[i*3+2]<<16;
		//i=0-->lcd_buf[0] = bmp_buf[0]<<0 | bmp_buf[1]<<8 | bmp_buf[2]<<16;
		//i=1-->lcd_buf[1] = bmp_buf[3]<<0 | bmp_buf[4]<<8 | bmp_buf[5]<<16;
		//i=2-->lcd_buf[2] = bmp_buf[6]<<0 | bmp_buf[7]<<8 | bmp_buf[8]<<16;
		//i=3-->lcd_buf[3] = bmp_buf[9]<<0 | bmp_buf[10]<<8 | bmp_buf[11]<<16;
	}
	
	//创建映射关系，将DDR3中的数据通过映射空间映射到显存中显示图像。
	int *lcd_mmap;
	lcd_mmap = mmap( NULL, 
				800*480*4,
				PROT_READ|PROT_WRITE,
				MAP_SHARED,
                fd_lcd, 
				0);
	
	//算法2：解决图片颠倒问题
	for(y=0; y<480; y++)
	{
		for(x=0; x<800; x++)
		{
			*(lcd_mmap+(479-y)*800+x) = lcd_buf[y*800+x];
		}		
	}
				  
	//4，回收申请的资源给系统。（close，munmap）
	close(fd_bmp);
	close(fd_lcd);	
	munmap(lcd_mmap, 800*480*4);	
	
	return 0;
}
