CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -O0
CFLAGS += -DHEX_XOR

TARGET = vm

SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

# 增量编译
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

