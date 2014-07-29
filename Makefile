CC        = gcc

SETUP     = etc/setup.sh

SRCDIR    = src
BINDIR    = bin
TARGET    = $(BINDIR)/integer-fhe

CFLAGS    = -Wall -g3 -ggdb -std=c99 -I/usr/local/include -I/usr/local/include/libflint -L/usr/local/lib
LDFLAGS  += -lgmp -lflint
SOURCES   = $(shell find $(SRCDIR) -name '*.c')
OBJECTS   = $(SOURCES:.c=.o)
.PHONY:    clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	chmod +x $(SETUP)
	./$(SETUP)
	mkdir -p $(BINDIR) 
	$(CC) -o $(TARGET) $(SOURCES) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OBJECTS) integer-fhe
	rm -r lib/*
