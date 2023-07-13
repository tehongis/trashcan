

CFLAGS := `sdl2-config --libs --cflags` -ggdb3 -O0 --std=c99 -Wall -lSDL2_image -lSDL2_mixer -lm		

soybean.exe	:	main.c

			gcc  -o soybean.exe main.c -DBUILD_NUMBER="\"$$(date)\"" $(CFLAGS)

run			: soybean.exe
			./soybean.exe

clean		:	soybean.exe
			rm -rf soybean.exe
