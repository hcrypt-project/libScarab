CC        = gcc

SRCDIR    = src
BINDIR    = bin
BUILDDIR  = build
TARGET    = $(BINDIR)/integer-fhe
LIBRARY   = $(BUILDDIR)/libscarab.so

CFLAGS    = -Wall -g3 -ggdb -std=c99 -I/usr/local/include -I/usr/local/include/libflint -L/usr/local/lib
LDFLAGS  += -lgmp -lflint
SOURCES   = $(shell find $(SRCDIR) -name '*.c')
OBJECTS   = $(SOURCES:.c=.o)
.PHONY:    clean

all: $(TARGET) $(LIBRARY)

$(TARGET): $(OBJECTS)
	mkdir -p $(BINDIR) 
	$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) integer-fhe
	rm -r lib/*
	rm -r $(LIBRARY)

$(LIBRARY):
	$(CC) -fPIC -shared -o $(LIBRARY) $(SOURCES) $(CFLAGS) $(LDFLAGS)
