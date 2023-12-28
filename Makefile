##
## Project : RType
## File : Makefile
##

all:	build

build:
	cmake -S . -B ./build
	make --no-print-directory -C build

debug:
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug
	make --no-print-directory -C build

run:
	cmake -DRUN=on -S . -B ./build
	make --no-print-directory -C build run

test:
	cmake -DTESTS=on -S . -B ./build
	make --no-print-directory -C build tests-run

package:
	cmake -DPACKAGE=on -S . -B ./build
	make --no-print-directory -C build

clean:
	rm -rf ./build/*

fclean: clean
	rm -rf ./lib/*
	rm -rf ./bin/*

re: fclean build

.PHONY: all build run test clean fclean re
