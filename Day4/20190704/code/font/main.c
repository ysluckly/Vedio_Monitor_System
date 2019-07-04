#include <stdio.h>
#include "font.h"

int main(void)
{
	//显示"000-张三-李四"字符串
	Init_Font();

	
	Clean_Area(200,//x坐标起始点
                 0,//y坐标起始点
				 400,//绘制的宽度
				 200,//绘制的高度
				 0x00000000);//往屏幕指定区域填充黑颜色
				 
	Display_characterX(200,//x坐标起始点
                        0,//y坐标起始点
						"000-张三-李四",//GB2312 中文字符串
						0x00ffffff ,//字体白颜色值
						2);//字体放大倍数 1~8

	UnInit_Font();	
	return 0;
}