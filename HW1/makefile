CC = gcc
CFLAGS = -Wall -g
TARGET = findword

OBJS = main.o findword.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

main.o: main.c findword.h
	$(CC) $(CFLAGS) -c main.c

findword.o: findword.c findword.h
	$(CC) $(CFLAGS) -c findword.c

clean:
	rm -f $(TARGET) $(OBJS)
