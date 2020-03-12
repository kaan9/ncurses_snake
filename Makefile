CC = gcc
CFLAGS = -Wall -c 
LDFLAGS = -lncurses
OBJS = snake.o curse.o
OUT = snake


all:	clean $(OUT)

$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT)

snake.o: 
	$(CC) $(CFLAGS) snake.c -o $@ 

curse.o:
	$(CC) $(CFLAGS) curse.c -o $@

clean:
	rm -f *.o snake
