test:
	make build;
	make run;

build:
	gcc -std=c99 -c sdl_gfx_test.c
	gcc sdl_gfx_test.o -o sdl_gfx_test -lSDL2 -lSDL2_gfx;

run:
	./sdl_gfx_test

clean:
	-rm sdl_gfx_test;
	-rm *.o;
