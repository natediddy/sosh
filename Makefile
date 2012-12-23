CC = gcc
CFLAGS = -Wall -O2 -march=native -mtune=native

TARGET = sosh
VERSION = 0.01

CFLAGS += -DSOSH_VERSION=\"$(VERSION)\"

SOURCES = sosh.c \
		  sosh-bltn.c \
		  sosh-env.c \
		  sosh-exec.c \
		  sosh-expr.c \
		  sosh-str.c \
		  sosh-var.c

OBJECTS = sosh.o \
		  sosh-bltn.o \
		  sosh-env.o \
		  sosh-exec.o \
		  sosh-expr.o \
		  sosh-str.o \
		  sosh-var.o

HEADERS = sosh.h \
		  sosh-bltn.h \
		  sosh-env.h \
		  sosh-exec.h \
		  sosh-expr.h \
		  sosh-str.h \
		  sosh-var.h

prefix = /usr/local

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

sosh.o: sosh.h
	$(CC) $(CFLAGS) -c sosh.c

sosh-bltn.o: sosh-bltn.h
	$(CC) $(CFLAGS) -c sosh-bltn.c

sosh-env.o: sosh-env.h
	$(CC) $(CFLAGS) -c sosh-env.c

sosh-exec.o: sosh-exec.h
	$(CC) $(CFLAGS) -c sosh-exec.c

sosh-var.o: sosh-var.h
	$(CC) $(CFLAGS) -c sosh-var.c

sosh-expr.o: sosh-expr.h
	$(CC) $(CFLAGS) -c sosh-expr.c

sosh-str.o: sosh-str.h
	$(CC) $(CFLAGS) -c sosh-str.c

clean:
	@rm -f $(OBJECTS)

clobber:
	@rm -f $(OBJECTS) $(TARGET)

install: $(TARGET)
	install -m 755 $(TARGET) $(prefix)/bin/$(TARGET)

uninstall:
	rm -f $(prefix)/bin/$(TARGET)

.PHONY: clean clobber install uninstall
