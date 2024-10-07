CC=gcc
CV=-std=c11
CFLAGS=-Wall -I.

ODIR=obj

_OBJ=tre.o termutil.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c
	$(CC) $(CFLAGS) $(CV) -c $< -o $@ 

tre: $(OBJ)
	$(CC) $(CFLAGS) $(CV) $^ -o $@

.PHONY: clean

run: tre
	./tre

clean:
	rm -rf $(ODIR)/*
