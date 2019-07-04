#include <stdio.h>
#include <time.h>
	
int main(void)
{
	char *p_time = NULL;
	time_t get_time;
	time(&get_time);	
	p_time = ctime(&get_time);
	
	printf("p_time=%s\n", p_time);
	
	return 0;
}