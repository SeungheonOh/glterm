CC = gcc

LIBS = `pkg-config --libs --cflags opengl glfw3 glu` -lm -lutil
CFLAGS = 

SRCS = main.c

TARGET = main

all: $(target)
	$(CC) -o $(TARGET) $(LIBS) $(SRCS)

run: all
	./main $(font)
