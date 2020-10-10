

SRC = $(wildcard src/*)
OBJECTS = $(foreach file, $(SRC), out/$(basename $(notdir $(file))).o)

CC = gcc
CFLAGS = -Wall -std=c99 -Iinc
OUTPUT = main.exe

all: out $(OBJECTS)
	@echo Building...
	@$(CC) -o $(OUTPUT) $(OBJECTS) -lm
	@echo Done

out:
	@mkdir -p $@

out/%.o: src/%.c
	@echo CC $<
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf out
