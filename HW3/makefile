CC = gcc
CFLAGS = -Wall -pthread
TARGET = mtws

all: $(TARGET)

$(TARGET): mtws.c
	$(CC) $(CFLAGS) -o $(TARGET) mtws.c

clean:
	rm -f $(TARGET)
