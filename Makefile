##
## Project : RType
## File : Makefile
##

SRC_DIR	=	src

all:	build

build:
	cmake -DDEBUG_MODE=on -S . -B ./build
	make --no-print-directory -C build

release:
	cmake -DDEBUG_MODE=off -S . -B ./build
	make --no-print-directory -C build

debug:
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug
	make --no-print-directory -C build

run:
	cmake -DRUN=on -DDEBUG_MODE=on -S . -B ./build
	make --no-print-directory -C build run

run_release:
	cmake -DRUN=on -DDEBUG_MODE=off -S . -B ./build
	make --no-print-directory -C build run

format:
	cp Cevy/.clang-format .
	@for src in $(shell find $(SRC_DIR) -name "*.cpp" -o -name "*.hpp") ; do \
		echo "Formatting [$$src]..." ;  			\
		clang-format -i "$$src" -style=file ; 		\
	done
	@echo "Done"

test:
	cmake -DTESTS=on -S . -B ./build
	make --no-print-directory -C build tests-run

clean:
	rm -rf ./build/*

fclean: clean
	rm -rf ./lib/*
	rm -rf ./bin/*

re: fclean build

.PHONY: all build run test clean fclean re
