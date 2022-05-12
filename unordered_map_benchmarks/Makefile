.PHONY: all clean

CXX=clang++

all: bench bench_debug

bench:
	mkdir -p build
	${CXX} -std=c++17 -O3 bench.cpp -o build/bench

bench_debug:
	mkdir -p build
	${CXX} -std=c++17 bench.cpp -o build/bench_debug

clean:
	rm -rf build
