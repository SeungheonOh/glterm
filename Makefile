CC = gcc

LIBS = `pkg-config --libs --cflags opengl glfw3 glu` -lm -lutil
CFLAGS = -g

SRCS = main.c

TARGET = main

all: $(target)
	$(CC) -o $(TARGET) $(LIBS) $(SRCS) $(CFLAGS)

debug: all 
	gdb $(TARGET)

run: all
	./main $(font)
