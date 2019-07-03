/*************************************************************************
	> File Name:    open.c
	> Author:       Hunter
	> Mail:         hunter.520@qq.com
	> Created Time: 2019年07月01日 星期一 02时56分27秒
 ************************************************************************/

#include<stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <string.h>

int main()
{
    int fd = open("./Hello.txt",O_CREAT | O_RDWR ,0777);
    if(fd < 0)
    {
        printf("CREAT is error\n");
    }
    printf("this FileScrib  is %d\n",fd);
  
    char p[]="Hello,Technology!!!\n";
    write(fd,p,sizeof(p));

    close(fd);

    int ffd = open("./Hello.txt", O_RDWR);
    char buf[1024];
    bzero(buf,sizeof(buf));
    ssize_t r = read(ffd,buf,sizeof(buf));
    if(r<0)
    {
        printf("read is eeror\n");
    }
    printf("The read is: %s\n",buf);

    close(ffd);
    return 0;
}
