#include "define_test_header.h"

int main(){
	struct registry_float reg_float;
	int rc = registry_create_float(&reg_float);
	registry_destroy_float(&reg_float);

	// var_collection_float coll_float;
	//rc = var_collection_create_float(&coll_float, &reg_float);

	return 0;
}
