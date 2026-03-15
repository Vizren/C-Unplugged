CC      = gcc
CFLAGS  = -std=c11 -Wall -Wextra -g

OBJS    = main.o song.o playlist.o queue.o storage.o cli.o log.o
TARGET  = cunplugged

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
