CC = gcc
CFLAGS = -Wall -O2 -march=native -mtune=native

TARGET = sosh
VERSION = 0.01

CFLAGS += -DSOSH_VERSION=\"$(VERSION)\"

SOURCES = sosh.c
OBJECTS = sosh.o
HEADERS = sosh.h

prefix = /usr/local

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

sosh.o: sosh.h
	$(CC) $(CFLAGS) -c sosh.c

clean:
	@rm -f $(OBJECTS)

clobber:
	@rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	install -m 755 $(TARGET) $(prefix)/bin/$(TARGET)

uninstall:
	rm -f $(prefix)/bin/$(TARGET)

.PHONY: clean clobber install uninstall
