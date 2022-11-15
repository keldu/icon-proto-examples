.PHONY: all generate_binding clean

CXX=g++
CC=gcc
CXXFLAGS=-std=c++20 -DNDEBUG -O3
FORTRAN=gfortran
FFLAGS=-cpp -lstdc++ -O3 -Wall -fbacktrace -std=f2018 -fcheck=all

all: define_test define_fortran_test binding.so

generate_binding:
	./generator/generate --root "./generator/"

binding.h: generate_binding

binding.cpp: generate_binding

binding.f90: generate_binding

binding.o: binding.cpp binding.h
	${CXX} ${CXXFLAGS} -I../ -c binding.cpp -o binding.o

binding.os: binding.cpp binding.h
	${CXX} ${CXXFLAGS} -I../ -fpic -c -o binding.os binding.cpp

binding.mod: binding.f90
	${FORTRAN} ${FFLAGS} -c -o binding.mod binding.o binding.f90

binding.so: binding.os
	${CXX} -shared -o binding.so binding.os

define_test: define_test.c binding.o
	${CXX} define_test.c binding.o -o define_test.bin

define_fortran_test: define_test.f90 binding.so binding.mod
	${FORTRAN} ${FFLAGS} binding.mod define_test.f90 -Wl,-rpath=. -L=. -l:binding.so -o define_test.fortran.bin

clean:
	rm *.so *.os *.o *.bin
