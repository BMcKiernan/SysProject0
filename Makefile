cc = gcc

# CFLAGS
CFLAGS = -g -Wall

SOURCES = sorter.c ADTList.c mergesort.c getcolumns.c readline.c
OBJECTS = $(SOURCES:.c=.o)
TARGET = sorter

#Magic
$(TARGET)  :$(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) core
