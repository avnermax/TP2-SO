CC = gcc
CFLAGS = -g -Wall
CLIBS = -lm
OBJS = funcoes.o io.o
TARGET = tp2virtual
all: $(TARGET)

tp2virtual: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o tp2virtual main.c $(CLIBS)
io.o: io.c
	$(CC) $(CFLAGS) -c io.c $(CLIBS)
funcoes.o: funcoes.c funcoes.h
	$(CC) $(CFLAGS) -c funcoes.c $(CLIBS)

clean:
	rm -rf $(OBJS) tp2virtual
