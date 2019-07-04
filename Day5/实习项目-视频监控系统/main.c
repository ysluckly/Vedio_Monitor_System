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
#include <time.h>
#include <pthread.h>
#include <string.h>
#include "fcntl.h"
#include "camera.h"
#include <unistd.h>
#include <strings.h>
#include <sys/mman.h>
#include <linux/input.h>//struct input_event

int fd_event0;
int fd_fb0;
int buf_bmp;
int buf_lcd;
int fd_bmp;
int fd_lcd;
int *p_init = NULL;
int *p_mmap = NULL;

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
void input_pageMAIN()
{
	//1）打开LCD屏幕，打开图片。
	fd_bmp = open("/root/project1/image/main.bmp",O_RDWR);
	fd_lcd = open("/dev/fb0",O_RDWR);
	if(fd_bmp < 0 || fd_lcd <0)
	{
        printf("FAILD !!! BMP Filescip iS %d AND LCD Filescip is %d \n",fd_bmp,fd_lcd);
        return -1;
    }

    printf("fd_bmp Filescip iS %d \nfd_lcd Filescip is %d \n",fd_bmp,fd_lcd);
	int ret = lseek(fd_bmp, 54, SEEK_SET);
	if (ret < 0)
	{
		printf("lseek error %d\n",ret);
		return -2;
	}
	
	//2）创建映射通道，将内存中图片的颜色数据通过p_mmap放到LCD屏幕。
	
	
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
}

//arm6818_bmp800_480("/root/project/images/0.bmp", 10, 10, 40, 40);
int arm6818_bmp800_480(const char *pathname, int coordinate_x, int coordinate_y, int height, int wide)
{
	int fd_bmp = open(pathname, O_RDWR);
	if(fd_bmp == -1)
	{
		printf("open bmp fail!\n");
		return -1;
	}
	
	lseek(fd_bmp, 54, SEEK_SET);
	
	char bmp_buf[height*wide*3];
	int lcd_buf[height*wide];
	bzero(bmp_buf, sizeof(bmp_buf));
	bzero(lcd_buf, sizeof(lcd_buf));
	
	read(fd_bmp, bmp_buf, sizeof(bmp_buf));
	close(fd_bmp);
	
	int i;
	char A = 0x00;
	for(i=0; i<height*wide; i++)
	{
		lcd_buf[i] = bmp_buf[i*3]<<0 | bmp_buf[i*3+1]<<8 | bmp_buf[i*3+2]<<16 | A<<24;
	}
	
	int *p_mmap = NULL;
	p_mmap = p_init;
	p_mmap = p_mmap + 800* coordinate_y + coordinate_x;

	//printf("Debug 1\n");	
	
	int x, y;
	for(y=0; y<height; y++)
	{
		for(x=0; x<wide; x++)
		{
			*(p_mmap + 800*(height-1-y) + x) = lcd_buf[wide*y+x];
		}
	}
	
	//printf("Debug 2\n");	
	
	p_mmap = NULL;
	
	return 0;
}
int close_fd(void)
{
	close(fd_bmp);
	close(fd_fb0);
	close(fd_lcd);
	close(fd_event0);
	return 0;
}

void* task_ser(void* arg)
{
	while(1)
	{
		//获取时间戳转换为时间
		char *p_time = NULL;
		time_t get_time;
		time(&get_time);	
		p_time = ctime(&get_time);
		
		char buf[100];
		bzero(buf,sizeof(buf));
		strcpy(buf,p_time);
		
		//buf:  Thu Jul  4 11:12:10 UTC 2019
		char *p_f = NULL;
		p_f = strtok(buf, " ");

		
		char *p_second = NULL;//"Jul" 月
		p_second = strtok(NULL, " ");
		
		
		char *p_third = NULL;//"4" 日
		p_third = strtok(NULL, " ");
		
		char *p_forth = NULL;  //时间
		p_forth = strtok(NULL, " ");
		
		
		// char *p_geshi = NULL;  //格式
		 // p_geshi = strtok(NULL, " ");
		 // printf("%s\n", p_geshi);

		char *p_five = NULL;//"2019"
		p_five = strtok(NULL, "\n");
		
		
		//Jul --> 
		int month=0;

		if(strcmp(p_second, "Jan") == 0) month = 1;
		else if(strcmp(p_second, "Feb") == 0) month = 2;
		else if(strcmp(p_second, "Mar") == 0) month = 3;
		else if(strcmp(p_second, "Apr") == 0) month = 4;
		else if(strcmp(p_second, "May") == 0) month = 5;
		else if(strcmp(p_second, "Jun") == 0) month = 6;
		else if(strcmp(p_second, "Jul") == 0) month = 7;
		else if(strcmp(p_second, "Aug") == 0) month = 8;
		else if(strcmp(p_second, "Sept") == 0) month = 9;
		else if(strcmp(p_second, "Oct") == 0) month = 10;
		else if(strcmp(p_second, "Nov") == 0) month = 11;
		else if(strcmp(p_second, "Dec") == 0) month = 12;
		
		char str[50];
		bzero(str, sizeof(str));
		sprintf(str, "%s-%d-%s %s", p_five, month, p_third,p_forth);
		
		//显示 "Jul  4 11:10:00 2019" 字符串
		Init_Font();
		
		Clean_Area(640,//x坐标起始点
					 0,//y坐标起始点
					 160,//绘制的宽度
					 15,//绘制的高度
					 0x00ffffff);//往屏幕指定区域填充黑颜色
					 
		Display_characterX(640,//x坐标起始点
							0,//y坐标起始点
							str,//GB2312 中文字符串
							0x00000000 ,//字体白颜色值
							1);//字体放大倍数 1~8

		UnInit_Font();	
		sleep(1);
	}
	pthread_exit(NULL);
}


int num = 3; //输入最多次数
int main()
{
	pthread_t pid;
	pthread_create(&pid,NULL,(void*)task_ser,NULL);
	
	
Build:
	input_pageMAIN();
	int ts_x, ts_y;
	
	open_ts();
	open_fb0();
	
	
	//登陆
	char account[7]="123456";
	char passwd[7]="123456";
	char input_account[11];
	char input_passwd[11];
FLAG:	
	bzero(input_account,sizeof(input_account));
	bzero(input_passwd,sizeof(input_passwd));
	
	
	while(1)
	{
		read_ts(&ts_x, &ts_y);
		if(ts_x>20&&ts_x<110&&ts_y>150&&ts_y<200)
		{
			printf("Please input Account:\n");
			
			//账号原点
		
			int ax=135;
			int ay= 152;
			int i = 0;
			while(1)
			{
FLAG1:				
				read_ts(&ts_x, &ts_y);
				//-->
				if(ts_x>20&&ts_x<110&&ts_y>245&&ts_y<395)
				{
					input_account[i] = '\0';
					printf("The Account  is %s\n",input_account);
					break;
				}
				//<--
				else if(ts_x>410&&ts_x<500&&ts_y>380&&ts_y<460)
				{
					ax-=40;
					if(ax < 135)
					{
						bzero(input_account,sizeof(input_account));
						ax=135;
						i=0;
						goto FLAG1;
						
					}
					arm6818_bmp800_480("/root/project1/image/q.bmp",ax,ay,40,40);
					--i;
					printf("delete a number!!!\n");
					
				}
				else if(ts_x>400&&ts_x<500&&ts_y>30&&ts_y<110)
				{
					arm6818_bmp800_480("/root/project1/image/num1.bmp",ax,ay,40,40);
					ax+=40;
					input_account[i++]='1';
					printf("1\n");
				}
				else if(ts_x>550&&ts_x<650&&ts_y>30&&ts_y<110)
				{
					arm6818_bmp800_480("/root/project1/image/num2.bmp",ax,ay,40,40);
					ax+=40;
					input_account[i++]='2';
					printf("2\n");
				}
				else if(ts_x>690&&ts_x<790&&ts_y>30&&ts_y<110)
				{
					arm6818_bmp800_480("/root/project1/image/num3.bmp",ax,ay,40,40);
					ax+=40;
					input_account[i++]='3';
					printf("3\n");
				}
				else if(ts_x>400&&ts_x<500&&ts_y>140&&ts_y<250)
				{
					arm6818_bmp800_480("/root/project1/image/num4.bmp",ax,ay,40,40);
					ax+=40;
					input_account[i++]='4';
					printf("4\n");
				}
				else if(ts_x>550&&ts_x<650&&ts_y>140&&ts_y<250)
				{
					arm6818_bmp800_480("/root/project1/image/num5.bmp",ax,ay,40,40);
					ax+=40;
					input_account[i++]='5';
					printf("5\n");
				}
				else if(ts_x>690&&ts_x<790&&ts_y>140&&ts_y<250)
				{
					arm6818_bmp800_480("/root/project1/image/num6.bmp",ax,ay,40,40);
					ax+=40;
					input_account[i++]='6';
					printf("6\n");
				}
				else if(ts_x>400&&ts_x<500&&ts_y>260&&ts_y<360)
				{
					arm6818_bmp800_480("/root/project1/image/num7.bmp",ax,ay,40,40);
					ax+=40;
					input_account[i++]='7';
					printf("7\n");
				}
				else if(ts_x>550&&ts_x<650&&ts_y>260&&ts_y<360)
				{
					arm6818_bmp800_480("/root/project1/image/num8.bmp",ax,ay,40,40);
					ax+=40;
					input_account[i++]='8';
					printf("8\n");
				}
				else if(ts_x>690&&ts_x<790&&ts_y>260&&ts_y<360)
				{
					arm6818_bmp800_480("/root/project1/image/num9.bmp",ax,ay,40,40);
					ax+=40;
					input_account[i++]='9';
					printf("9\n");
				}	
			}
			if(ts_x>20&&ts_x<110&&ts_y>245&&ts_y<395)
			{
				printf("Please input Passwd:\n");
				//密码原点
		
				int px=135;
				int py= 250;
				int i = 0;
				while(1)
				{
FLAG2:
					read_ts(&ts_x, &ts_y);
					//--->
					if(ts_x>700&&ts_x<790&&ts_y>380&&ts_y<470)
					{
						input_passwd[i]='\0';
						printf("Input Passwd is %s\n",input_passwd);
						num--;
						if(num >0)
						{
							//校验账号密码是否正确
							if((strcmp(account,input_account)==0)&&(strcmp(passwd,input_passwd)==0))
							{
								printf("*************************Welcome to this syatem!!!************************\n");
								arm6818_color(0x0000ff00,0,0,480,800);
								sleep(1);
Buildwww:
								//第二个界面
								arm6818_bmp800_480("/root/project1/image/zhongjian.bmp",0,0,480,800);
								while(1)
								{
									read_ts(&ts_x, &ts_y);
									if(ts_x>330&&ts_x<460&&ts_y>290&&ts_y<430)
									{
										//第三个界面
										arm6818_bmp800_480("/root/project1/image/right.bmp",0,0,480,800); 
										while(1)
										{
											read_ts(&ts_x, &ts_y);
											if(ts_x>640&&ts_x<800&&ts_y>160&&ts_y<250)
											{
												//TODO  监控
											
												my_camera();
												ts_x=0;
												ts_y=0;

											}
											if(ts_x>640&&ts_x<800&&ts_y>360&&ts_y<440)
											{
												printf("------------------------Thank You !!! --------------------\n");
												num=3;
												goto Buildwww;
											}
											
										}
									}
									if(ts_x>120&&ts_x<250&&ts_y>290&&ts_y<430)
									{
										printf("------------------------Thank You !!! --------------------\n");
										num=3;
										goto Build;
									}
								}
								
							}
							else
							{
								arm6818_color(0x00ff0000,0,0,480,800);
								sleep(1);
								arm6818_bmp800_480("/root/project1/image/main.bmp",0,0,480,800);
								goto FLAG;
								printf("Passwd is error \n!!!");
							}

						}
						else
						{
							if((strcmp(account,input_account)==0)&&(strcmp(passwd,input_passwd)==0))
							{
								printf("*************************Welcome to this syatem!!!**********************\n");
								arm6818_color(0x0000ff00,0,0,480,800);
								sleep(1);
Buildwwww:
								arm6818_bmp800_480("/root/project1/image/zhongjian.bmp",0,0,480,800);
								while(1)
								{
									read_ts(&ts_x, &ts_y);
									if(ts_x>330&&ts_x<460&&ts_y>290&&ts_y<430)
									{
										//第三个界面
										arm6818_bmp800_480("/root/project1/image/right.bmp",0,0,480,800); 
										while(1)
										{
											
											read_ts(&ts_x, &ts_y);
											if(ts_x>640&&ts_x<800&&ts_y>160&&ts_y<250)
											{
												//TODO  监控
												
												my_camera();
												ts_x=0;
												ts_y=0;	

											}
											if(ts_x>640&&ts_x<800&&ts_y>360&&ts_y<440)
											{
												printf("------------------------Thank You !!! --------------------\n");
												num=3;
												goto Buildwwww;
											}
											
										}
									}
									if(ts_x>120&&ts_x<250&&ts_y>290&&ts_y<430)
									{
										printf("------------------------Thank You !!! --------------------\n");
										num=3;
										goto Build;
									}
									
								}
							}
							else
							{
								printf("**********************DEV IS SHUTDOWN  NOW!!!************************\n");
	
								arm6818_color(0x00000000,0,0,480,800);
							}
							
						}
						
						return 0;
					}
					//<--
					else if(ts_x>410&&ts_x<500&&ts_y>380&&ts_y<460)
					{
						px-=40;
						if(px < 0)
						{						
							bzero(input_passwd,sizeof(input_passwd));
							px=135;
							i = 0;
							goto FLAG2;
						}
						arm6818_bmp800_480("/root/project1/image/q.bmp",px,py,40,40);
						--i;
						printf("delete a number!!!\n");
					
					}
					else if(ts_x>400&&ts_x<500&&ts_y>30&&ts_y<110)
					{
						arm6818_bmp800_480("/root/project1/image/num10.bmp",px,py,40,40);
						px+=40;
						input_passwd[i++]='1';
						printf("1\n");
					}
					else if(ts_x>550&&ts_x<650&&ts_y>30&&ts_y<110)
					{
						arm6818_bmp800_480("/root/project1/image/num10.bmp",px,py,40,40);
						px+=40;
						input_passwd[i++]='2';
						printf("2\n");
					}
					else if(ts_x>690&&ts_x<790&&ts_y>30&&ts_y<110)
					{
						arm6818_bmp800_480("/root/project1/image/num10.bmp",px,py,40,40);
						px+=40;
						input_passwd[i++]='3';
						printf("3\n");
					}
					else if(ts_x>400&&ts_x<500&&ts_y>140&&ts_y<250)
					{
						arm6818_bmp800_480("/root/project1/image/num10.bmp",px,py,40,40);
						px+=40;
						input_passwd[i++]='4';
						printf("4\n");
					}
					else if(ts_x>550&&ts_x<650&&ts_y>140&&ts_y<250)
					{
						arm6818_bmp800_480("/root/project1/image/num10.bmp",px,py,40,40);
						px+=40;
						input_passwd[i++]='5';
						printf("5\n");
					}
					else if(ts_x>690&&ts_x<790&&ts_y>140&&ts_y<250)
					{
						arm6818_bmp800_480("/root/project1/image/num10.bmp",px,py,40,40);
						px+=40;
						input_passwd[i++]='6';
						printf("6\n");
					}
					else if(ts_x>400&&ts_x<500&&ts_y>260&&ts_y<360)
					{
						arm6818_bmp800_480("/root/project1/image/num10.bmp",px,py,40,40);
						px+=40;
						input_passwd[i++]='7';
						printf("7\n");
					}
					else if(ts_x>550&&ts_x<650&&ts_y>260&&ts_y<360)
					{
						arm6818_bmp800_480("/root/project1/image/num10.bmp",px,py,40,40);
						px+=40;
						input_passwd[i++]='8';
						printf("8\n");
					}
					else if(ts_x>690&&ts_x<790&&ts_y>260&&ts_y<360)
					{
						arm6818_bmp800_480("/root/project1/image/num10.bmp",px,py,40,40);
						px+=40;
						input_passwd[i++]='9';
						printf("9\n");
					}	
				}
			
			}	
		}
		else
		{
			printf("Please input Account !!!\n");
		}
	}
	
	
	//3）关闭LCD屏幕，关闭图片，释放映射空间的资源。
	pthread_join(pid,NULL);
	exit_mmap();
	close_fd();	 
	
    return 0;
}
