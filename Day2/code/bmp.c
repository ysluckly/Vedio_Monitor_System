#include <stdio.h>

int main(void)
{
	//1）打开LCD屏幕，打开图片。
	
	//2）创建映射通道，将内存中图片的颜色数据通过p_mmap放到LCD屏幕。
	int *p_init = NULL;
	int *p_mmap = NULL;
	
	
	//2-1）800*480个BMP像素点变成800*480个LCD像素点
	
	//2-2）颠倒图片
	
	//3）关闭LCD屏幕，关闭图片，释放映射空间的资源。
	
	
	return 0;
}