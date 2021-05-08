TARGET=fs.exe
CC=gcc
AR=ar
CFLAGS=-std=c11 -Wall -pedantic -Wextra
LIBNAME=lib_fs.a
RM=del -f

.PHONY: all lib test clean

all: lib io.o
	$(CC) $(CFLAGS) io.o $(LIBNAME) -o $(TARGET)

lib: fs.o
	$(AR) -rsc $(LIBNAME) fs.o

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

test: lib tests.o
	$(CC) $(CFLAGS) tests.o $(LIBNAME) -o $(TARGET)
	$(TARGET)
	$(RM) $(TARGET)
	make clean

clean:
	$(RM) *.o $(LIBNAME)

