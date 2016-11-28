TinyJPEG:   Makefile
	arm-linux-gnueabi-gcc -std=c99 main.c tiny_jpeg.h  stb/stb_image.h -g -static -fstack-usage -o TinyJPEG -lm

gc:
	gcc main.c -std=c99 tiny_jpeg.h  stb/stb_image.h -g -static -fstack-usage -o TinyJPEG -lm
clean:
	rm -rf TinyJPEG *.su
