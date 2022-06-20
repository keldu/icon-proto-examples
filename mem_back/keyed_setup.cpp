#include "key_registry.h"

#include <string>
#include <iostream>

namespace imb {
using dynamic_registry_map = keyed_registry_map<uint32_t, std::string, uint32_t, uint16_t, float>;
}

void simple_keyed_setup(){
	
	imb::keyed_registry<uint32_t, std::string> registry;

	/// Builder
	auto builder = create_keyed_collection_builder<uint32_t, std::string>(registry);

	/// element isn't necessary, but provides the created index
	size_t element{};
	element = builder.add_var(1, "foo");
	element = builder.add_var(2, "bar");
	element = builder.add_var(3, "gaz");

	auto collection = builder.build();

	collection.for_each([](uint32_t key, const std::string& value){
		std::cout<<"Key: "<<key<<" Value: "<<value<<std::endl;
	});
}

void multi_registry_setup(){
	
	imb::dynamic_registry_map registry_map;
	{
		std::cout<<"Temperature test"<<std::endl;

		registry_map.add_registry<float>("temperature");

		auto registry_ptr = registry_map.find_registry_variant("temperature");
		if(!registry_ptr){
			return;
		}
		auto& registry_var = *registry_ptr;
		auto& registry = std::get<imb::keyed_registry<uint32_t,float>>(registry_var);

		auto builder = create_keyed_collection_builder(registry);

		size_t element{};
		element = builder.add_var(1, 5.0);
		element = builder.add_var(2, 3.0);
		element = builder.add_var(3, 2.0);
		element = builder.add_var(4, 10.0);
		element = builder.add_var(5, 2.5);

		auto collection = builder.build();

		collection.for_each([](uint32_t key, float value){
			std::cout<<"Key: "<<key<<" Value: "<<value<<std::endl;
		});

		// Sort
		//
		auto sorted_collection = collection.sort([](const std::pair<uint32_t, float>& left, const std::pair<uint32_t,float>& right){
			return left.second < right.second;
		});
		
		std::cout<<"\nSorted collection\n\n";
		sorted_collection.for_each([](uint32_t key, float value){
			std::cout<<"Key: "<<key<<" Value: "<<value<<std::endl;
		});

		auto filtered_collection = collection.filter([](const std::pair<uint32_t, float>& val){
			return val.second > 2.6;
		});
		
		std::cout<<"\nFiltered collection\n\n";
		filtered_collection.for_each([](uint32_t key, float value){
			std::cout<<"Key: "<<key<<" Value: "<<value<<std::endl;
		});

	}


}

int main(){
	std::cout<<"\n## Simple setup ##\n"<<std::endl;

	simple_keyed_setup();
	
	std::cout<<"\n## Multi registry ##\n"<<std::endl;
	multi_registry_setup();
	
	return 0;
}
