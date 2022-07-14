.PHONY: all build_dir clean

CXX=clang++
CXXFLAGS=-std=c++17 -DNDEBUG -O3 
#CXXFLAGS=-std=c++17 -g

NVCC=nvcc

### make rules ###

all: basic keyed descriptor

build_dir:
	mkdir -p build

basic: basic_setup.cpp build_dir
	${CXX} ${CXXFLAGS} -o build/basic_setup basic_setup.cpp

keyed: keyed_setup.cpp build_dir
	${CXX} ${CXXFLAGS} -o build/keyed_setup keyed_setup.cpp

descriptor: descriptor_setup.cpp build_dir
	${CXX} ${CXXFLAGS} -o build/descriptor_setup descriptor_setup.cpp

cuda: cuda/setup.cpp build_dir
	${NVCC} ${CXXFLAGS} -o build/cuda_setup cuda/setup.cpp

clean:
	rm -rf build
