#include <stdio.h>#include "camera.h"int main(int argc, char *argv[]){	open_ts();		show_bmp800_480("/root/project/images/2.bmp");	my_camera();	close_ts();	return 0;}