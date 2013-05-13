CFLAGS=-std=c99 -Wall -g -O2

.PHONY: all
all: mandelbrot mandelbrot.bmp

.PHONY: clean
clean:
	rm -f mandelbrot mandelbrot.bmp

mandelbrot: mandelbrot.c
	$(CC) $(CFLAGS) $(shell sdl-config --cflags) -o $@ $< $(shell sdl-config --libs) -lm

mandelbrot.bmp: mandelbrot
	./mandelbrot
