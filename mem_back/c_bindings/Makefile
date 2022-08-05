.PHONY: all clean

CXX=g++
CC=gcc

all: define_test

define_test_impl.o: define_test_impl.cpp capi_define_helper.h
	${CXX} -std=c++17 define_test_impl.cpp -c -o define_test_impl.o

define_test: define_test.c define_test_impl.o
	${CC} define_test.c define_test_impl.o -o define_test.bin

clean:
	rm *.o *.bin
