all: chip8

chip8: chip8.c
	gcc -o chip8  chip8.c chip8lib.c

clean:
	rm chip8 *.o
