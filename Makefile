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

.PHONY: all build test
