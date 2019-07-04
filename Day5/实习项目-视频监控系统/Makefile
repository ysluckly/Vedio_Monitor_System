main:main.c camera.c

	arm-linux-gcc -w $^ -o $@ -I./jpeg -L./jpeg -ljpeg -lapi_v4l2_arm  -L./ -lfont -lpthread
.PHONY:clean
clean:
	rm -rf main
