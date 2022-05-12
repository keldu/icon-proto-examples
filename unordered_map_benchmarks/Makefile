.PHONY: all clean

CXX ?= clang++

bench:
	mkdir -p build
	${CXX} -std=c++17 bench.cpp -o build/bench

all: bench

clean:
	rm -rf build
