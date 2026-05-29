CC      = gcc
CFLAGS  = -std=c99 -Wall -Wextra -O2
LIBS    = -lm
TARGET  = calculator

SRCS = main.c lexer.c parser.c variables.c history.c functions.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run