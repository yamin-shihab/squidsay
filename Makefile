# pls don't make fun of my pathetic makefile

CFLAGS := -std=c99 -pedantic -Wall -Wextra -Ofast -o build/squidsay

all: build

build:
	mkdir -p build && $(CC) src/squidsay.c $(CFLAGS)

install: build
	cp build/squidsay /usr/local/bin

clean:
	rm -rf build

.PHONY: clean build install
