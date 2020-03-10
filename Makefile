CC = tcc
CFLAGS = -Wall -c 
LDFLAGS = -lncurses
OBJS = snake.o
OUT = snake


all:	clean $(OUT)

$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT)

snake.o: 
	$(CC) $(CFLAGS) snake.c -o $@ 

clean:
	rm -f *.o snake
