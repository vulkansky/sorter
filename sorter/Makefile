CC := gcc
CFLAGS := -Wall -Wextra -Werror

SRCS := sorter.c platform/Linux/unix_dependency.c

OBJS := $(SRCS:.c=.o)

TARGET := sorter

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c sorter.h platform/Linux/unix_dependency.h
	$(CC) $(CFLAGS) -I. -Iplatform/Linux -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
