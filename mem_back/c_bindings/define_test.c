#include "generator/binding.h"

void iterate(float value){
	(void) value;
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

	descriptor_registry_context_destroy(&reg_ctx);

	return 0;
}
