##
## Project : RType
## File : Makefile
##

all:	build

build:
	cmake -S . -B ./build
	make --no-print-directory -C build

run:
	cmake -DRUN=on -S . -B ./build
	make --no-print-directory -C build run

test:
	cmake -DTESTS=on -S . -B ./build
	make --no-print-directory -C build tests-run

clean:
	rm -rf ./build/*

fclean: clean
	rm -rf ./lib/*
	rm -rf ./bin/*

re: fclean build

.PHONY: all build test
