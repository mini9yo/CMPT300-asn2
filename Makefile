CC = gcc
CFLAGS = -Wall -Wextra -g -pthread
LDFLAGS = -pthread

SRCS = main.c sockets.c list.c receiveThread.c printThread.c sendThread.c inputThread.c
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
TARGET = s-talk

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	$(CC) -MMD $(CFLAGS) $< -o $@

-include $(DEPS)

clean:
	$(RM) $(TARGET) $(OBJS) $(DEPS)
