#include "binding.h"


#include <stdio.h>

void iterate(const struct var_descriptor* key, float value){
	printf("Value: %f\n", value);
}

int filter(const struct var_descriptor* key, float value){
	return value < 5.f;
}

int filter_two(const struct var_descriptor* key, float value){
	return value > 9.f;
}

int sort(const struct var_descriptor* key_l, float value_l, const struct var_descriptor* key_r, float value_r){
	return value_l < value_r;
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

	// Dummy desc for now. Too lazy to set values for now. Unique desc are generated in the add_var functionality
	struct var_descriptor desc;
	size_t id = descriptor_registry_float_add_var(&reg_float, &desc, 5.f);
	id = descriptor_registry_float_add_var(&reg_float, &desc, 6.f);
	id = descriptor_registry_float_add_var(&reg_float, &desc, 2.f);
	id = descriptor_registry_float_add_var(&reg_float, &desc, 3.f);
	id = descriptor_registry_float_add_var(&reg_float, &desc, 10.f);
	id = descriptor_registry_float_add_var(&reg_float, &desc, 11.f);
	id = descriptor_registry_float_add_var(&reg_float, &desc, 1.f);

	struct var_collection_float collection;

	var_collection_float_create_global(&collection, &reg_float);
	
	printf("Print all values\n");
	var_collection_float_for_each(&collection, &iterate);

	struct var_collection_float filtered;

	var_collection_float_filter(&filtered, &collection, &filter);

	printf("\nPrint filtered values\n");
	var_collection_float_for_each(&filtered, &iterate);
	
	var_collection_float filtered_two;

	var_collection_float_filter(&filtered_two, &collection, &filter_two);
	printf("\nPrint other filtered values\n");
	var_collection_float_for_each(&filtered_two, &iterate);
	
	struct var_collection_float concat;
	var_collection_float_concat(&concat, &filtered, &filtered_two);
	
	printf("\nPrint concat values\n");
	var_collection_float_for_each(&concat, &iterate);
	
	struct var_collection_float sorted;
	var_collection_float_sort(&sorted, &collection, &sort);

	printf("\nPrint sorted values\n");
	var_collection_float_for_each(&sorted, &iterate);
	
	var_collection_float_destroy(&sorted);
	var_collection_float_destroy(&collection);
	var_collection_float_destroy(&filtered);
	var_collection_float_destroy(&filtered_two);
	descriptor_registry_context_destroy(&reg_ctx);

	return 0;
}
