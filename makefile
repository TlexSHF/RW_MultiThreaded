#MAKEFILE Oppgave 6 TRÅDER

#Extendable flags
INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)
CFLAGS += -lpthread

OBJDIR = obj

#All header files
_DEPS = main.h
DEPS = $(patsubst %, $(INCLDIR)/%, $(_DEPS))

#All object/source files
_OBJS = main.o
OBJS = $(patsubst %, $(OBJDIR)/%, $(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

#NameOfProject:
RW_Multithreaded: $(OBJS)
	gcc -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*~
