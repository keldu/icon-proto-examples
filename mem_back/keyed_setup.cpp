#include "var_descriptor.h"
#include "key_registry.h"
#include "descriptor_registry.h"

#include <array>
#include <string>
#include <iostream>

namespace imb {

template<typename... T>
struct registry_types {
	using keyed = keyed_registry_map<var_location, T...>;

	using descriptor = descriptor_registry_map<T...>;
};

// Technically the var descriptor should be here or my variant since the registry description acts as an internal 
using dynamic_registries = registry_types<
	std::string,
	
	uint8_t, uint16_t, uint32_t, uint64_t,
	int8_t, int16_t, int32_t, int64_t,
	float, double,

	std::array<uint8_t,2>, std::array<uint8_t,3>, std::array<uint8_t,4>,
	std::array<uint16_t,2>, std::array<uint16_t,3>, std::array<uint16_t,4>,
	std::array<uint32_t,2>, std::array<uint32_t,3>, std::array<uint32_t,4>,
	std::array<uint64_t,2>, std::array<uint64_t,3>, std::array<uint64_t,4>,

	std::array<int8_t,2>, std::array<int8_t,3>, std::array<int8_t,4>,
	std::array<int16_t,2>, std::array<int16_t,3>, std::array<int16_t,4>,
	std::array<int32_t,2>, std::array<int32_t,3>, std::array<int32_t,4>,
	std::array<int64_t,2>, std::array<int64_t,3>, std::array<int64_t,4>,
	
	std::array<float,2>, std::array<float,3>, std::array<float,4>,
	std::array<double,2>, std::array<double,3>, std::array<double,4>
>;

using dynamic_registry_map = dynamic_registries::keyed;
}

void simple_keyed_setup(){
	
	imb::keyed_registry<uint32_t, std::string> registry;

	/// Builder
	auto builder = imb::create_keyed_collection_builder<uint32_t, std::string>(registry);

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

		auto registry_ptr = registry_map.template find_registry<float>("temperature");
		if(!registry_ptr){
			return;
		}
		auto& registry = *registry_ptr;
		// auto& registry = std::get<imb::keyed_registry<imb::var_location,float>>(registry_var);

		auto builder = create_keyed_collection_builder(registry);

		size_t element{};
		element = builder.add_var({1,1,1,1}, 5.0);
		element = builder.add_var({1,2,3,4}, 3.0);
		element = builder.add_var({1,1,1,2}, 2.0);
		element = builder.add_var({1,1,1,3}, 10.0);
		element = builder.add_var({1,1,1,4}, 2.5);

		auto collection = builder.build();

		collection.for_each([](imb::var_location key, float value){
			std::cout<<"Key: "<<key.time_id<<" Value: "<<value<<std::endl;
		});

		// Sort
		//
		auto sorted_collection = collection.sort([](const std::pair<imb::var_location, float>& left, const std::pair<imb::var_location,float>& right){
			return left.second < right.second;
		});
		
		std::cout<<"\nSorted collection\n\n";
		sorted_collection.for_each([](imb::var_location key, float value){
			std::cout<<"Key: "<<key.time_id<<" Value: "<<value<<std::endl;
		});

		auto filtered_collection = collection.filter([](const imb::var_location& key, float& val){
			return val > 2.6;
		});
		
		std::cout<<"\nFiltered collection\n\n";
		filtered_collection.for_each([](imb::var_location key, float value){
			std::cout<<"Key: "<<key.time_id<<" Value: "<<value<<std::endl;
		});

		// Concat two functions
		std::cout<<"\nConcat collection\n\n";

		auto filter_sort_collection = sorted_collection.filter([](const imb::var_location& key,float& val){
			return val > 2.4 && val < 7.0;
		});
		auto concat_collection = filter_sort_collection.concat(filtered_collection);
		concat_collection.for_each([](imb::var_location key, float value){
			std::cout<<"Key: "<<key.time_id<<" Value: "<<value<<std::endl;
		});
	}


}

void multi_descriptor_setup(){
	imb::dynamic_registries::descriptor registry_map;
	{
		std::cout<<"Temperature test"<<std::endl;

		registry_map.add_registry<float>("temperature");

		// auto 

	}
}

int main(){
	std::cout<<"\n## Simple setup ##\n"<<std::endl;

	simple_keyed_setup();
	
	std::cout<<"\n## Multi registry ##\n"<<std::endl;
	multi_registry_setup();
	
	std::cout<<"\n## Multi descriptor registry ##\n"<<std::endl;
	multi_descriptor_setup();
	
	return 0;
}
