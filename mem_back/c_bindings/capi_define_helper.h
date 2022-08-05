#pragma once

#include <stddef.h>
#include <stdlib.h>

#define IMB_CAPI_BEGIN \
extern "C"{

#define IMB_CAPI_END \
}

#define IMB_CAPI_TYPE_IMPL(TYPE) \
int var_collection_create_##TYPE ( struct var_collection_##TYPE* collection, size_t size){ \
	if(collection == 0L){\
		return -1;\
	}\
\
	void* ptr = malloc(sizeof(size_t) * size);\
	if(ptr) {\
		return -1;\
	}\
	collection->ids = (size_t*)ptr;\
	collection->size = size; \
	return 0;\
}\
\
void var_collection_destroy_##TYPE ( struct var_collection_##TYPE* collection) {\
	free(collection);\
} \
\
int registry_create_##TYPE ( struct registry_##TYPE* registry) { \
	void* ptr = malloc(sizeof(imb::registry<float>)); \
	if(!ptr){ \
		return -1; \
	} \
	registry->internal_ptr = ptr; \
	return 0; \
} \
\
void registry_destroy_##TYPE ( struct registry_##TYPE* registry) { \
	if(!registry) { \
		return; \
	} \
	free(registry->internal_ptr); \
}

#define IMB_CAPI_TYPE_HEADERS(TYPE) \
struct registry_##TYPE { \
	void* internal_ptr; \
}; \
\
extern int registry_create_##TYPE ( struct registry_##TYPE * collection); \
extern void registry_destroy_##TYPE ( struct registry_##TYPE * collection); \
\
struct var_collection_##TYPE { \
	size_t size; \
	size_t* ids; \
}; \
extern int var_collection_create_##TYPE ( struct var_collection_##TYPE* collection, size_t size); \
extern void var_collection_destroy_##TYPE ( struct var_collection_##TYPE* collection);
