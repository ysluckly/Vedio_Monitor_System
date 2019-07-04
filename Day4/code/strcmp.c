#include <stdio.h>//printf
#include <string.h>//strcmp

int main(void)
{
	char login_account[7] = "123456";
	char login_password[7] = "123456";
	
	char input_password[7] = "12345";
	
	if(strcmp(login_password, input_password) == 0)
	{
		printf("equal!\n");
	}
	else{
		printf("no equal!\n");		
	}

}