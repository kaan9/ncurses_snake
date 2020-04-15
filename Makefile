CC = gcc
CFLAGS = -Wall -c 
LDFLAGS = -lncurses
OBJS = snake.o curse.o
OUT = snake

all:	clobber $(OUT) clean

$(OUT): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(OUT)

snake.o: 
	$(CC) $(CFLAGS) snake.c -o $@ 

curse.o:
	$(CC) $(CFLAGS) curse.c -o $@

clean:
	rm -f *.o

clobber:
	rm -f *.o $(OUT)

run:	all
	./$(OUT)

format: 
	clang-format -i *.c *.h

.PHONY: clean clobber run
