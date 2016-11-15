minimp3:   Makefile
	arm-linux-gnueabi-gcc libc.h minimp3.c player_oss.c spm_management.h -static -fstack-usage -o minimp3 -lm

gc:
	gcc libc.h minimp3.c player_oss.c spm_management.h -static -fstack-usage -o minimp3 -lm
clean:
	rm -rf minimp3 *.su
