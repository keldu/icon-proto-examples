#pragma once

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct var_descriptor {
	const char* name;
	int32_t jg;
	int32_t hgrid_id;
	int32_t vgrid_id;
	int32_t time_id;
};

struct descriptor_registry_ctx {
	void* internal;
};

extern int descriptor_registry_context_create( struct descriptor_registry_ctx* ctx );

extern void descriptor_registry_context_destroy( struct descriptor_registry_ctx* ctx );


struct descriptor_registry_float {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_float_create ( struct descriptor_registry_float* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_float_destroy ( struct descriptor_registry_float* registry );

extern size_t descriptor_registry_float_add_var ( struct descriptor_registry_float* registry, float variable);

struct var_collection_float {
	void* internal = 0L;
	struct descriptor_registry_float* registry = 0L;
};

extern int var_collection_float_create ( struct var_collection_float* collection, struct descriptor_registry_float* registry);

extern int var_collection_float_create_global ( struct var_collection_float* collection, struct descriptor_registry_float* registry);

extern void var_collection_float_destroy ( struct var_collection_float* collection);

extern void var_collection_float_for_each ( struct var_collection_float* collection, void (*func)( const struct var_descriptor*, float ) );

extern void var_collection_float_filter ( struct var_collection_float* target, struct var_collection_float* collection, int (*func)( const struct var_descriptor*, float ) );

extern void var_collection_float_concat ( struct var_collection_float* target, struct var_collection_float* left, struct var_collection_float* right);

struct descriptor_registry_double {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_double_create ( struct descriptor_registry_double* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_double_destroy ( struct descriptor_registry_double* registry );

extern size_t descriptor_registry_double_add_var ( struct descriptor_registry_double* registry, double variable);

struct var_collection_double {
	void* internal = 0L;
	struct descriptor_registry_double* registry = 0L;
};

extern int var_collection_double_create ( struct var_collection_double* collection, struct descriptor_registry_double* registry);

extern int var_collection_double_create_global ( struct var_collection_double* collection, struct descriptor_registry_double* registry);

extern void var_collection_double_destroy ( struct var_collection_double* collection);

extern void var_collection_double_for_each ( struct var_collection_double* collection, void (*func)( const struct var_descriptor*, double ) );

extern void var_collection_double_filter ( struct var_collection_double* target, struct var_collection_double* collection, int (*func)( const struct var_descriptor*, double ) );

extern void var_collection_double_concat ( struct var_collection_double* target, struct var_collection_double* left, struct var_collection_double* right);

struct descriptor_registry_uint8_t {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_uint8_t_create ( struct descriptor_registry_uint8_t* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_uint8_t_destroy ( struct descriptor_registry_uint8_t* registry );

extern size_t descriptor_registry_uint8_t_add_var ( struct descriptor_registry_uint8_t* registry, uint8_t variable);

struct var_collection_uint8_t {
	void* internal = 0L;
	struct descriptor_registry_uint8_t* registry = 0L;
};

extern int var_collection_uint8_t_create ( struct var_collection_uint8_t* collection, struct descriptor_registry_uint8_t* registry);

extern int var_collection_uint8_t_create_global ( struct var_collection_uint8_t* collection, struct descriptor_registry_uint8_t* registry);

extern void var_collection_uint8_t_destroy ( struct var_collection_uint8_t* collection);

extern void var_collection_uint8_t_for_each ( struct var_collection_uint8_t* collection, void (*func)( const struct var_descriptor*, uint8_t ) );

extern void var_collection_uint8_t_filter ( struct var_collection_uint8_t* target, struct var_collection_uint8_t* collection, int (*func)( const struct var_descriptor*, uint8_t ) );

extern void var_collection_uint8_t_concat ( struct var_collection_uint8_t* target, struct var_collection_uint8_t* left, struct var_collection_uint8_t* right);

struct descriptor_registry_uint16_t {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_uint16_t_create ( struct descriptor_registry_uint16_t* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_uint16_t_destroy ( struct descriptor_registry_uint16_t* registry );

extern size_t descriptor_registry_uint16_t_add_var ( struct descriptor_registry_uint16_t* registry, uint16_t variable);

struct var_collection_uint16_t {
	void* internal = 0L;
	struct descriptor_registry_uint16_t* registry = 0L;
};

extern int var_collection_uint16_t_create ( struct var_collection_uint16_t* collection, struct descriptor_registry_uint16_t* registry);

extern int var_collection_uint16_t_create_global ( struct var_collection_uint16_t* collection, struct descriptor_registry_uint16_t* registry);

extern void var_collection_uint16_t_destroy ( struct var_collection_uint16_t* collection);

extern void var_collection_uint16_t_for_each ( struct var_collection_uint16_t* collection, void (*func)( const struct var_descriptor*, uint16_t ) );

extern void var_collection_uint16_t_filter ( struct var_collection_uint16_t* target, struct var_collection_uint16_t* collection, int (*func)( const struct var_descriptor*, uint16_t ) );

extern void var_collection_uint16_t_concat ( struct var_collection_uint16_t* target, struct var_collection_uint16_t* left, struct var_collection_uint16_t* right);

struct descriptor_registry_uint32_t {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_uint32_t_create ( struct descriptor_registry_uint32_t* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_uint32_t_destroy ( struct descriptor_registry_uint32_t* registry );

extern size_t descriptor_registry_uint32_t_add_var ( struct descriptor_registry_uint32_t* registry, uint32_t variable);

struct var_collection_uint32_t {
	void* internal = 0L;
	struct descriptor_registry_uint32_t* registry = 0L;
};

extern int var_collection_uint32_t_create ( struct var_collection_uint32_t* collection, struct descriptor_registry_uint32_t* registry);

extern int var_collection_uint32_t_create_global ( struct var_collection_uint32_t* collection, struct descriptor_registry_uint32_t* registry);

extern void var_collection_uint32_t_destroy ( struct var_collection_uint32_t* collection);

extern void var_collection_uint32_t_for_each ( struct var_collection_uint32_t* collection, void (*func)( const struct var_descriptor*, uint32_t ) );

extern void var_collection_uint32_t_filter ( struct var_collection_uint32_t* target, struct var_collection_uint32_t* collection, int (*func)( const struct var_descriptor*, uint32_t ) );

extern void var_collection_uint32_t_concat ( struct var_collection_uint32_t* target, struct var_collection_uint32_t* left, struct var_collection_uint32_t* right);

struct descriptor_registry_uint64_t {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_uint64_t_create ( struct descriptor_registry_uint64_t* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_uint64_t_destroy ( struct descriptor_registry_uint64_t* registry );

extern size_t descriptor_registry_uint64_t_add_var ( struct descriptor_registry_uint64_t* registry, uint64_t variable);

struct var_collection_uint64_t {
	void* internal = 0L;
	struct descriptor_registry_uint64_t* registry = 0L;
};

extern int var_collection_uint64_t_create ( struct var_collection_uint64_t* collection, struct descriptor_registry_uint64_t* registry);

extern int var_collection_uint64_t_create_global ( struct var_collection_uint64_t* collection, struct descriptor_registry_uint64_t* registry);

extern void var_collection_uint64_t_destroy ( struct var_collection_uint64_t* collection);

extern void var_collection_uint64_t_for_each ( struct var_collection_uint64_t* collection, void (*func)( const struct var_descriptor*, uint64_t ) );

extern void var_collection_uint64_t_filter ( struct var_collection_uint64_t* target, struct var_collection_uint64_t* collection, int (*func)( const struct var_descriptor*, uint64_t ) );

extern void var_collection_uint64_t_concat ( struct var_collection_uint64_t* target, struct var_collection_uint64_t* left, struct var_collection_uint64_t* right);

struct descriptor_registry_int8_t {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_int8_t_create ( struct descriptor_registry_int8_t* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_int8_t_destroy ( struct descriptor_registry_int8_t* registry );

extern size_t descriptor_registry_int8_t_add_var ( struct descriptor_registry_int8_t* registry, int8_t variable);

struct var_collection_int8_t {
	void* internal = 0L;
	struct descriptor_registry_int8_t* registry = 0L;
};

extern int var_collection_int8_t_create ( struct var_collection_int8_t* collection, struct descriptor_registry_int8_t* registry);

extern int var_collection_int8_t_create_global ( struct var_collection_int8_t* collection, struct descriptor_registry_int8_t* registry);

extern void var_collection_int8_t_destroy ( struct var_collection_int8_t* collection);

extern void var_collection_int8_t_for_each ( struct var_collection_int8_t* collection, void (*func)( const struct var_descriptor*, int8_t ) );

extern void var_collection_int8_t_filter ( struct var_collection_int8_t* target, struct var_collection_int8_t* collection, int (*func)( const struct var_descriptor*, int8_t ) );

extern void var_collection_int8_t_concat ( struct var_collection_int8_t* target, struct var_collection_int8_t* left, struct var_collection_int8_t* right);

struct descriptor_registry_int16_t {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_int16_t_create ( struct descriptor_registry_int16_t* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_int16_t_destroy ( struct descriptor_registry_int16_t* registry );

extern size_t descriptor_registry_int16_t_add_var ( struct descriptor_registry_int16_t* registry, int16_t variable);

struct var_collection_int16_t {
	void* internal = 0L;
	struct descriptor_registry_int16_t* registry = 0L;
};

extern int var_collection_int16_t_create ( struct var_collection_int16_t* collection, struct descriptor_registry_int16_t* registry);

extern int var_collection_int16_t_create_global ( struct var_collection_int16_t* collection, struct descriptor_registry_int16_t* registry);

extern void var_collection_int16_t_destroy ( struct var_collection_int16_t* collection);

extern void var_collection_int16_t_for_each ( struct var_collection_int16_t* collection, void (*func)( const struct var_descriptor*, int16_t ) );

extern void var_collection_int16_t_filter ( struct var_collection_int16_t* target, struct var_collection_int16_t* collection, int (*func)( const struct var_descriptor*, int16_t ) );

extern void var_collection_int16_t_concat ( struct var_collection_int16_t* target, struct var_collection_int16_t* left, struct var_collection_int16_t* right);

struct descriptor_registry_int32_t {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_int32_t_create ( struct descriptor_registry_int32_t* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_int32_t_destroy ( struct descriptor_registry_int32_t* registry );

extern size_t descriptor_registry_int32_t_add_var ( struct descriptor_registry_int32_t* registry, int32_t variable);

struct var_collection_int32_t {
	void* internal = 0L;
	struct descriptor_registry_int32_t* registry = 0L;
};

extern int var_collection_int32_t_create ( struct var_collection_int32_t* collection, struct descriptor_registry_int32_t* registry);

extern int var_collection_int32_t_create_global ( struct var_collection_int32_t* collection, struct descriptor_registry_int32_t* registry);

extern void var_collection_int32_t_destroy ( struct var_collection_int32_t* collection);

extern void var_collection_int32_t_for_each ( struct var_collection_int32_t* collection, void (*func)( const struct var_descriptor*, int32_t ) );

extern void var_collection_int32_t_filter ( struct var_collection_int32_t* target, struct var_collection_int32_t* collection, int (*func)( const struct var_descriptor*, int32_t ) );

extern void var_collection_int32_t_concat ( struct var_collection_int32_t* target, struct var_collection_int32_t* left, struct var_collection_int32_t* right);

struct descriptor_registry_int64_t {
	struct descriptor_registry_ctx* ctx = 0L;
	char* name = 0L;
};

extern int descriptor_registry_int64_t_create ( struct descriptor_registry_int64_t* registry, const char* name, struct descriptor_registry_ctx* registry_ctx );

extern void descriptor_registry_int64_t_destroy ( struct descriptor_registry_int64_t* registry );

extern size_t descriptor_registry_int64_t_add_var ( struct descriptor_registry_int64_t* registry, int64_t variable);

struct var_collection_int64_t {
	void* internal = 0L;
	struct descriptor_registry_int64_t* registry = 0L;
};

extern int var_collection_int64_t_create ( struct var_collection_int64_t* collection, struct descriptor_registry_int64_t* registry);

extern int var_collection_int64_t_create_global ( struct var_collection_int64_t* collection, struct descriptor_registry_int64_t* registry);

extern void var_collection_int64_t_destroy ( struct var_collection_int64_t* collection);

extern void var_collection_int64_t_for_each ( struct var_collection_int64_t* collection, void (*func)( const struct var_descriptor*, int64_t ) );

extern void var_collection_int64_t_filter ( struct var_collection_int64_t* target, struct var_collection_int64_t* collection, int (*func)( const struct var_descriptor*, int64_t ) );

extern void var_collection_int64_t_concat ( struct var_collection_int64_t* target, struct var_collection_int64_t* left, struct var_collection_int64_t* right);


#ifdef __cplusplus
}
#endif
