SHELl := /bin/sh
DBG = -g -DDEBUG
FLAGS = -Wall -Wextra
LIB = -lpthread
CC = gcc
SRCS = $(wildcard src/*.c *.c)
ELFS = $(SRCS:.c=.o)
EXE = $(SRCS:.c=)
DEPS = .dep

all: $(EXE)
	@mkdir elfs
	@mv $(ELFS) elfs

$(EXE): $(ELFS)
	$(CC) $(FLAGS) $(DBG) -o $(EXE) $^ $(LIB)

%.o: %.c
	$(CC) $(FLAGS) $(DBG) -c $<

clean:
	rm -rf $(ELFS) elfs/ $(DEPS) $(EXE)

debug:
	$(CC) $(FLAGS) $(DBG) -o $(EXE) $(SRCS) $(LIB)

dep: $(SRCS)
	$(CC) -M $(FLAGS) $< > .$@


sinclude .dep
.PHONY: all clean dep
