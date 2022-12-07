PHONY: all build_dir clean

all: example

generated.h: generate.h.tmpl
	python3 generate_wp

example: example.c binding.c generated.h.tmpl
	gcc example.c binding.c -o example

build_dir:
	mkdir -p build/

clean:
	rm -rf build/
