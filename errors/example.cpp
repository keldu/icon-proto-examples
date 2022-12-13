#include "error.h"

#include <iostream>

struct FooError {
	static constexpr std::string_view name = "Foo";
	static constexpr bool is_critical = false;
};

struct BarError {
	static constexpr std::string_view name = "Bar";
	static constexpr bool is_critical = true;
};

int main(){
	using namespace imb;

	error foo_err = make_error<FooError> ( "Some stuff and text" );
	error bar_err = make_error<BarError> ( "Some other stuff and text" );

	std::cout << foo_err << std::endl;
	std::cout << bar_err << std::endl;

	return 0;
}
