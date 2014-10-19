CC        = gcc

SRCDIR    = src
BINDIR    = bin
BUILDDIR  = build

PREFIX_LIB = /usr/local/lib

TARGET    = $(BINDIR)/integer-fhe

LIBRARY_FILENAME = libscarab.so
LIBRARY   = $(BUILDDIR)/$(LIBRARY_FILENAME)


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
	rm -r $(BINDIR) $(BUILDDIR)

$(LIBRARY):
	mkdir -p $(BUILDDIR)
	$(CC) -fPIC -shared -o $(LIBRARY) $(SOURCES) $(CFLAGS) $(LDFLAGS)

install:
	cp $(LIBRARY) $(PREFIX_LIB)/
	ldconfig

uninstall:
	rm $(PREFIX_LIB)/$(LIBRARY_FILENAME)
	ldconfig
