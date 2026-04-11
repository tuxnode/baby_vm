CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -O0 -Iinclude
CFLAGS += -DHEX_XOR

TARGET = vm

SRC_DIR = src

SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
