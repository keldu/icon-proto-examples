#include "binding.h"

#include <stdio.h>

void print_var_float64 ( double x )
{
	printf("This is a double: %f", x);
}

void print_var_float32 ( float x )
{
	printf("This is a float: %f", x);
}

void print_bucket ( ) {
	printf("This is a bucket");
}
