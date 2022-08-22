#include "generator/binding.h"

#include <stdio.h>

void iterate(const void* key, float value){
	printf("Value: %f\n", value);
}

int main(){
	struct descriptor_registry_ctx reg_ctx;
	int rc = descriptor_registry_context_create(&reg_ctx);
	if(rc != 0){
		return -1;
	}

	struct descriptor_registry_float reg_float;
	rc = descriptor_registry_float_create(&reg_float, "temperature", &reg_ctx);
	if(rc != 0){
		return -2;
	}

	size_t id = descriptor_registry_float_add_var(&reg_float, 5.f);
	id = descriptor_registry_float_add_var(&reg_float, 6.f);
	id = descriptor_registry_float_add_var(&reg_float, 2.f);
	id = descriptor_registry_float_add_var(&reg_float, 3.f);
	id = descriptor_registry_float_add_var(&reg_float, 10.f);
	id = descriptor_registry_float_add_var(&reg_float, 11.f);
	id = descriptor_registry_float_add_var(&reg_float, 1.f);

	struct var_collection_float collection;

	var_collection_float_create_global(&collection, &reg_float);
	
	var_collection_float_for_each(&collection, &iterate);

	descriptor_registry_context_destroy(&reg_ctx);

	return 0;
}
