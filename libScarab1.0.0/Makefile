CC       = gcc
CFLAGS   = -Wall -g3 -ggdb -std=c99 -I/usr/local/include -I/usr/local/include/libflint -L/usr/local/lib
LDFLAGS += -lgmp -lflint
SOURCE   = $(shell find * -name '*.c')
OBJECTS  = $(SOURCE:.c=.o)
.PHONY:    clean

all: integer-fhe

integer-fhe: $(OBJECTS)
	$(CC) -o integer-fhe $^ $(LDFLAGS)

clean:
	rm -f $(OBJECTS) integer-fhe

