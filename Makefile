CC=gcc
CV=-std=c11
CFLAGS=-Wall -MMD -I. -g

ODIR=obj
SRCDIR=src

SRC_FILES=$(wildcard $(SRCDIR)/*.c)
OBJ=$(patsubst $(SRCDIR)/%.c,$(ODIR)/%.o,$(SRC_FILES))

$(ODIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(ODIR)
	$(CC) $(CFLAGS) $(CV) -c $< -o $@ 

tre: $(OBJ)
	$(CC) $(CFLAGS) $(CV) $^ -o $@

.PHONY: clean

clean:
	rm -rf $(ODIR)/*
	rm -f ./tre
