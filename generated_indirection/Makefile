PHONY: all clean

all: example

generated.h:
	python3 generate_wp --type double --name float64

example: example.c binding.c generated.h
	gcc example.c binding.c -o example

clean:
	rm -f example generated.h
