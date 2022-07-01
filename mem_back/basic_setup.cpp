#include "./static/registry.h"

#include <iostream>

using global_registry = imb::registry_tuple<uint64_t>;

int main(){
	using namespace imb;

	global_registry global_reg;

	/// Build a collection
	auto builder = create_collection_builder(global_reg.template get_registry<uint64_t>());

	/// element isn't necessary, but provides the created index
	size_t element{};
	element = builder.add_var(2);
	element = builder.add_var(5);
	element = builder.add_var(3);
	element = builder.add_var(7);
	element = builder.add_var(40);
	element = builder.add_var(345);
	element = builder.add_var(100);

	auto collection = builder.build();

	collection.for_each([](size_t value){
		std::cout<<"Value: \t"<<value<<"\n";
	});
	
	std::cout<<"\n";
	
	/// Sorting
	auto sorted_collection = collection.sort([](const uint64_t& foo, const uint64_t& bar) -> bool{
		return foo < bar;
	});
	
	sorted_collection.for_each([](uint64_t value){
		std::cout<<"Sorted Value: \t"<<value<<"\n";
	});
	
	std::cout<<"\n";

	/// Filtering
	auto filtered_collection = collection.filter([](const uint64_t& val){
		return val > 2 && val < 200;
	});
	
	filtered_collection.for_each([](uint64_t value){
		std::cout<<"Filter Value: \t"<<value<<"\n";
	});
	
	std::cout<<"\n";
	
	/// Combined
	collection.filter([](uint64_t val){
		return val > 1 && val < 100;
	}).sort([](uint64_t left, uint64_t right){
		return left > right;
	}).for_each([](uint64_t val){
		std::cout<<"Comb Value: \t"<<val<<"\n";
	});

	std::cout<<std::endl;

	return 0;
}
