CC = gcc
CFLAGS = -Wall -O2 -march=native -mtune=native

TARGET = sosh
VERSION = 0.01

CFLAGS += -DSOSH_VERSION=\"$(VERSION)\"

SOURCES = sosh.c \
		  sosh-builtins.c \
		  sosh-env.c \
		  sosh-external.c

OBJECTS = sosh.o \
		  sosh-builtins.o \
		  sosh-env.o \
		  sosh-external.o

HEADERS = sosh.h \
		  sosh-builtins.h \
		  sosh-env.h \
		  sosh-external.h

prefix = /usr/local

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

sosh.o: sosh.h
	$(CC) $(CFLAGS) -c sosh.c

sosh-builtins.o: sosh-builtins.h
	$(CC) $(CFLAGS) -c sosh-builtins.c

sosh-env.o: sosh-env.h
	$(CC) $(CFLAGS) -c sosh-env.c

sosh-external.o: sosh-external.h
	$(CC) $(CFLAGS) -c sosh-external.c

clean:
	@rm -f $(OBJECTS)

clobber:
	@rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	install -m 755 $(TARGET) $(prefix)/bin/$(TARGET)

uninstall:
	rm -f $(prefix)/bin/$(TARGET)

.PHONY: clean clobber install uninstall
