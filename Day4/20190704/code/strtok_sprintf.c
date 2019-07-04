#include <stdio.h>
#include <string.h>
	
int main(void)
{
	char time[50] = "Wed Jul  3 18:39:12 2019";
	
	char *p_first = NULL;
	p_first = strtok(time, " ");
	printf("%s\n", p_first);
	
	char *p_second = NULL;//"Jul"
	p_second = strtok(NULL, " ");
	printf("%s\n", p_second);	
	
	char *p_third = NULL;//"3"
	p_third = strtok(NULL, " ");
	printf("%s\n", p_third);
	
	char *p_forth = NULL;
	p_forth = strtok(NULL, " ");
	printf("%s\n", p_forth);
	
	char *p_five = NULL;//"2019"
	p_five = strtok(NULL, " ");
	printf("%s\n", p_five);	
	
	//Jul --> 7
	int month;
	if(strcmp(p_second, "Jul") == 0) month = 7;
	
	char string[50];
	bzero(string, sizeof(string));
	sprintf(string, "%s-%d-%s", p_five, month, p_third);
	printf("string=%s\n", string);
	
	return 0;
}