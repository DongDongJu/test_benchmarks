TinyJPEG:   Makefile
	arm-linux-gnueabi-gcc -std=c99 main.c spm_management.h tiny_jpeg.h  stb/stb_image.h -g -static -fstack-usage -o TinyJPEG -lm

gc:
	gcc -std=c99 main.c spm_management.h tiny_jpeg.h  stb/stb_image.h -g -static -fstack-usage -o TinyJPEG -lm
clean:
	rm -rf TinyJPEG *.su
