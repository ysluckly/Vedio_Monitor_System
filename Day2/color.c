/*************************************************************************
	> File Name:    color.c
	> Author:       Hunter
	> Mail:         hunter.520@qq.com
	> Created Time: 2019年07月01日 星期一 19时50分17秒
 ************************************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>


int main()
{
    int fd_fb0=open("/dev/fb0",O_RDWR);
    
    if(fd_fb0 < 0)
    {
       printf("error -1");
       return -1;
    }
    else
    {

        printf("This FileScip is %d\n",fd_fb0);
        int color = 0x00ffff00;
        int *p_mmap = NULL;
        int *p_init = NULL;

        p_init = mmap(NULL,800*480*4,PROT_READ | PROT_WRITE, MAP_SHARED , fd_fb0,0);
        if(p_init == MAP_FAILED)
        {
            printf("error map\n");
            return -2;
        }
        
        p_mmap = p_init;

        int i = 0;
        for( i = 0; i < 800*480*4; ++p_mmap)
        { 
            *p_mmap = color;

            ++i;
        }
        
        munmap(p_init, 800*480*4);
        p_mmap= NULL;
        p_init = NULL;
        close(fd_fb0);
    }
    return  0;
}
