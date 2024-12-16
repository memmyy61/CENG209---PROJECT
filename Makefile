CC=gcc
CFLAGS=-Wall -Wextra -g

# Source files
SRCS=main.c commands.c game.c

# Header files
HEADERS=commands.h game.h

# Output executable
TARGET=minecraft_dungeons.exe

# Build rules
all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	del /f /q $(TARGET)
	del /f /q *.o

.PHONY: all clean
