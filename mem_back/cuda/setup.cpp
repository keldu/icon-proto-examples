#include "./registry.h"

#include <iostream>

using cuda_basic_registry = imb::registry<uint64_t,imb::storage::cuda>;

int main(){
	using namespace imb;

	cuda_basic_registry cuda_reg;

	auto builder = create_collection_builder<uint64_t>(cuda_reg);

	builder.add_var(2);
	builder.add_var(5);
	builder.add_var(20);
	builder.add_var(13123);

	auto collection = builder.build();

	collection.for_each([](uint64_t value){
		std::cout<<"Value: \t"<<value<<"\n";
	});

	return 0;
}
