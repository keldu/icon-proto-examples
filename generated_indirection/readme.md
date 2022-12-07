# Some explanations  

`example.c` is just the main program. `generated.h.tmpl` contains the to be generated indirection.
`binding.h` and `binding.c` are technically also the generated bindings, but the generation isn't necessary in this case.  

# Dependencies  

* python3
* jinja2
* make
* gcc

# Compile  

* `make example`

# Bugs  

* forgot how to make the dependencies correctly on the header from the template. Workaround is to call `make clean` as well
