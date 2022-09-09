.PHONY: all generate_binding clean

CXX=g++
CC=gcc
CXXFLAGS=-std=c++20 -DNDEBUG -O3

all: define_test binding.so

generate_binding:
	./generator/generate --root "./generator/"

binding.h: generate_binding

binding.cpp: generate_binding

binding.o: binding.cpp binding.h
	${CXX} ${CXXFLAGS} -I../ -c binding.cpp -o binding.o

binding.os: binding.cpp binding.h
	${CXX} ${CXXFLAGS} -I../ -fpic -c -o binding.os binding.cpp

binding.so: binding.os
	${CXX} -shared -o binding.so binding.os

define_test: define_test.c binding.o
	${CXX} define_test.c binding.o -o define_test.bin

clean:
	rm *.so *.os *.o *.bin
