#include "descriptor_registry.h"

#include <iostream>

void simple_setup(){
	imb::descriptor_registry_map<float, int32_t> map;

	map.template add_registry<float>("temperature");

	{
		
	}
}

int main(){
	std::cout<<"\n## Simple setup ##\n"<<std::endl;

	simple_setup();

	return 0;
}
