# change application name here (executable output name)
TARGET=producer

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-D_XOPEN_SOURCE=700
# warnings
WARN=-Wall

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

LIB=-lrt

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(LIB) -export-dynamic

OBJS=    main.o

all:	$(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
    
main.o:	src/main.c
	$(CC) -c $(CCFLAGS) src/main.c $(LIB) -o main.o
    
clean:
	rm -f *.o $(TARGET)
