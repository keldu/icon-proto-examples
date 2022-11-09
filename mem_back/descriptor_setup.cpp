#include "descriptor_registry.h"

#include <cassert>
#include <iostream>

void simple_setup(){
	std::cout<<"\n## Simple setup ##\n"<<std::endl;
	imb::descriptor_registry_map<float, int32_t> map;

	map.template add_registry<float>("temperature");
	

	{
		auto reg_ptr = map.template find_registry<float>("temperature");
		if(!reg_ptr){
			return;
		}

		auto& registry = *reg_ptr;
		auto builder = imb::create_keyed_collection_builder<imb::var_location, float>(registry);

		builder.add_var({1,1,1,1}, 5.0);
		builder.add_var({1,2,3,4}, 3.0);
		builder.add_var({1,1,1,2}, 2.0);
		builder.add_var({1,1,1,3}, 10.0);
		builder.add_var({1,1,1,4}, 2.5);
		
		auto opt_collection = map.template global_collection_of<float>("temperature");

		if( ! opt_collection.has_value() ){
			return;
		}

		auto& collection = *opt_collection;

		collection.for_each(
			[](const imb::var_descriptor& desc, float val){
				std::cout<<"Descriptor: \""<<desc.name<<"\" ";
				std::cout<<desc.location.jg<<" ";
				std::cout<<desc.location.hgrid_id<<" ";
				std::cout<<desc.location.vgrid_id<<" ";
				std::cout<<desc.location.time_id<<" ";
				std::cout<<"Value: "<<val<<"\n";
			}
		);
		std::cout.flush();
	}

	map.template add_registry<int32_t>("some_sort_of_id");

	{
		auto reg_ptr = map.template find_registry<int32_t>("some_sort_of_id");
		if(!reg_ptr){
			return;
		}

		auto& registry = *reg_ptr;
		auto builder = imb::create_keyed_collection_builder<imb::var_location, int32_t>(registry);

		builder.add_var({1,1,1,1}, 2);
		builder.add_var({1,1,1,2}, 3);
		builder.add_var({1,1,1,4}, 4);
		builder.add_var({1,1,1,5}, 5);
		builder.add_var({1,1,1,6}, 6);
		builder.add_var({1,1,2,1}, 7);
		builder.add_var({1,1,3,1}, 8);
	}
	
	std::cout<<"\n## Variant setup ##\n"<<std::endl;

	{
		auto var_col = map.global_collection_variant();

		var_col.for_each(
			[](imb::var_descriptor& desc, std::variant<float*, int32_t*> var_val){
				std::visit([&desc](auto val){
					assert(val);
					
					std::cout<<"Descriptor: \""<<desc.name<<"\" ";
					std::cout<<desc.location.jg<<" ";
					std::cout<<desc.location.hgrid_id<<" ";
					std::cout<<desc.location.vgrid_id<<" ";
					std::cout<<desc.location.time_id<<" ";

					std::cout<<"Value: "<<*val<<std::endl;
				}, var_val);
			}
		);
	}
}

int main(){

	simple_setup();
	

	return 0;
}
