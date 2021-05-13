SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

LDFLAGS = -lncurses
CCFLAGS = -g -Wall

TARGET = db8080

$(TARGET): $(OBJ)
	mkdir -p bin
	$(CC) $(LDFLAGS) $(CCFLAGS) -o bin/$@ $^

.PHONY: clean
clean:
	rm -f $(OBJ) bin/$(TARGET)

