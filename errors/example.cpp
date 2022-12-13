#include "error.h"

#include <iostream>

struct FooError {};

struct BarError {};

int main(){
	using namespace imb;

	auto id = error_registry::template get_or_new_error_id <FooError> ("Foo", true);
	auto id2 = error_registry::template get_or_new_error_id <BarError> ("Bar", true);
	auto id3 = error_registry::template get_or_new_error_id <FooError> ("Foo", true);

	std::cout << "Foo: " << id << std::endl;
	std::cout << "Bar: " << id2 << std::endl;
	std::cout << "Foo: " << id3 << std::endl;

	std::cout << "\n";



	error foo_err = make_error<FooError> ( "Some stuff and text" );
	error bar_err = make_error<BarError> ( "Some other stuff and text" );

	std::cout << foo_err << std::endl;
	std::cout << bar_err << std::endl;

	return 0;
}
