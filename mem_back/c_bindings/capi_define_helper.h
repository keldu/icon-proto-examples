#pragma once

#define IMB_CAPI_BEGIN \
extern "C"{

#define IMB_CAPI_END \
}

#define IMB_CAPI_TYPE_IMPL(TYPE) \
int var_collection_create_##TYPE (var_collection_##TYPE* collection, size_t size){ \
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
void var_collection_destroy_##TYPE (var_collection_##TYPE* collection) {\
	free(collection);\
} \
\
int registry_create_##TYPE (registry_##TYPE* registry) { \
	void* ptr = malloc(sizeof(imb::registry<float>)); \
	if(!ptr){ \
		return -1; \
	} \
	registry->internal_ptr = ptr; \
	return 0; \
} \
\
void registry_destroy_##TYPE (registry_##TYPE* registry) { \
	if(!registry) { \
		return; \
	} \
	free(registry->internal_ptr); \
} \
\

#define IMB_CAPI_TYPE_HEADERS(TYPE) \
struct var_collection_##TYPE { \
	size_t size; \
	size_t* ids; \
}; \
int var_collection_create_##TYPE (var_collection_##TYPE* collection, size_t size); \
void var_collection_destroy_##TYPE (var_collection_##TYPE* collection); \
\
struct registry_##TYPE { \
	void* internal_ptr; \
}; \
\
int registry_create_##TYPE (registry_##TYPE* collection); \
void registry_destroy_##TYPE (registry_##TYPE* collection); \
\

