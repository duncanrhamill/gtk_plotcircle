CC=gcc

plotcircle: plotcircle.c
	$(CC) -o plotcircle plotcircle.c `pkg-config --cflags gtk+-3.0 --libs gtk+-3.0`