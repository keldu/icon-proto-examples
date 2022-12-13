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

	return 0;
}
