#include "./binding.h"

#include "descriptor_registry.h"

namespace imb {
using descriptor_registry_map_spec = descriptor_registry_map<
	float,
	double,
	uint8_t,
	uint16_t,
	uint32_t,
	uint64_t,
	int8_t,
	int16_t,
	int32_t,
	int64_t
>;

}

#include <limits>

#include <cassert>

#include <cstring>



extern "C" {

int descriptor_registry_context_create ( descriptor_registry_ctx* ctx ){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	ctx->internal = new (std::nothrow) imb::descriptor_registry_map_spec();

	if(!ctx->internal){
		return -1;
	}

	return 0;
}

void descriptor_registry_context_destroy ( descriptor_registry_ctx* ctx ){
	assert(ctx);
	if(!ctx){
		return;
	}

	auto ptr = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	delete ptr;
}
}
imb::keyed_registry< imb::var_location, float > * descriptor_registry_float_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < float > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_float_create( struct descriptor_registry_float* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< float > ( std::string{name} );

	auto internal = map->find_registry< float > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_float_destroy( struct descriptor_registry_float* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_float_add_var( struct descriptor_registry_float* registry, float value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_float_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_float_create ( struct var_collection_float* collection, struct descriptor_registry_float* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_float_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, float >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_float_create_global ( struct var_collection_float* collection, struct descriptor_registry_float* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_float_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, float >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_float_destroy ( struct var_collection_float* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, float >* > ( collection->internal );
}

void var_collection_float_for_each ( struct var_collection_float* collection, void (*func)( const struct var_descriptor*, float ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, float >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, float second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_float_filter ( struct var_collection_float* target, struct var_collection_float* collection, int (*func)( const struct var_descriptor*,  float ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, float >* >( collection->internal );

	var_collection_float_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, float >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, float second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_float_concat ( struct var_collection_float* target, struct var_collection_float* left, struct var_collection_float* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, float >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, float >* >( right->internal );

	var_collection_float_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, float >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
imb::keyed_registry< imb::var_location, double > * descriptor_registry_double_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < double > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_double_create( struct descriptor_registry_double* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< double > ( std::string{name} );

	auto internal = map->find_registry< double > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_double_destroy( struct descriptor_registry_double* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_double_add_var( struct descriptor_registry_double* registry, double value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_double_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_double_create ( struct var_collection_double* collection, struct descriptor_registry_double* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_double_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, double >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_double_create_global ( struct var_collection_double* collection, struct descriptor_registry_double* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_double_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, double >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_double_destroy ( struct var_collection_double* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, double >* > ( collection->internal );
}

void var_collection_double_for_each ( struct var_collection_double* collection, void (*func)( const struct var_descriptor*, double ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, double >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, double second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_double_filter ( struct var_collection_double* target, struct var_collection_double* collection, int (*func)( const struct var_descriptor*,  double ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, double >* >( collection->internal );

	var_collection_double_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, double >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, double second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_double_concat ( struct var_collection_double* target, struct var_collection_double* left, struct var_collection_double* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, double >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, double >* >( right->internal );

	var_collection_double_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, double >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
imb::keyed_registry< imb::var_location, uint8_t > * descriptor_registry_uint8_t_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < uint8_t > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_uint8_t_create( struct descriptor_registry_uint8_t* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< uint8_t > ( std::string{name} );

	auto internal = map->find_registry< uint8_t > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_uint8_t_destroy( struct descriptor_registry_uint8_t* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_uint8_t_add_var( struct descriptor_registry_uint8_t* registry, uint8_t value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_uint8_t_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_uint8_t_create ( struct var_collection_uint8_t* collection, struct descriptor_registry_uint8_t* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_uint8_t_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, uint8_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_uint8_t_create_global ( struct var_collection_uint8_t* collection, struct descriptor_registry_uint8_t* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_uint8_t_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, uint8_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_uint8_t_destroy ( struct var_collection_uint8_t* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint8_t >* > ( collection->internal );
}

void var_collection_uint8_t_for_each ( struct var_collection_uint8_t* collection, void (*func)( const struct var_descriptor*, uint8_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint8_t >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, uint8_t second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_uint8_t_filter ( struct var_collection_uint8_t* target, struct var_collection_uint8_t* collection, int (*func)( const struct var_descriptor*,  uint8_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint8_t >* >( collection->internal );

	var_collection_uint8_t_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint8_t >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, uint8_t second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_uint8_t_concat ( struct var_collection_uint8_t* target, struct var_collection_uint8_t* left, struct var_collection_uint8_t* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint8_t >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint8_t >* >( right->internal );

	var_collection_uint8_t_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint8_t >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
imb::keyed_registry< imb::var_location, uint16_t > * descriptor_registry_uint16_t_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < uint16_t > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_uint16_t_create( struct descriptor_registry_uint16_t* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< uint16_t > ( std::string{name} );

	auto internal = map->find_registry< uint16_t > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_uint16_t_destroy( struct descriptor_registry_uint16_t* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_uint16_t_add_var( struct descriptor_registry_uint16_t* registry, uint16_t value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_uint16_t_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_uint16_t_create ( struct var_collection_uint16_t* collection, struct descriptor_registry_uint16_t* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_uint16_t_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, uint16_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_uint16_t_create_global ( struct var_collection_uint16_t* collection, struct descriptor_registry_uint16_t* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_uint16_t_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, uint16_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_uint16_t_destroy ( struct var_collection_uint16_t* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint16_t >* > ( collection->internal );
}

void var_collection_uint16_t_for_each ( struct var_collection_uint16_t* collection, void (*func)( const struct var_descriptor*, uint16_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint16_t >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, uint16_t second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_uint16_t_filter ( struct var_collection_uint16_t* target, struct var_collection_uint16_t* collection, int (*func)( const struct var_descriptor*,  uint16_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint16_t >* >( collection->internal );

	var_collection_uint16_t_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint16_t >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, uint16_t second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_uint16_t_concat ( struct var_collection_uint16_t* target, struct var_collection_uint16_t* left, struct var_collection_uint16_t* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint16_t >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint16_t >* >( right->internal );

	var_collection_uint16_t_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint16_t >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
imb::keyed_registry< imb::var_location, uint32_t > * descriptor_registry_uint32_t_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < uint32_t > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_uint32_t_create( struct descriptor_registry_uint32_t* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< uint32_t > ( std::string{name} );

	auto internal = map->find_registry< uint32_t > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_uint32_t_destroy( struct descriptor_registry_uint32_t* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_uint32_t_add_var( struct descriptor_registry_uint32_t* registry, uint32_t value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_uint32_t_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_uint32_t_create ( struct var_collection_uint32_t* collection, struct descriptor_registry_uint32_t* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_uint32_t_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, uint32_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_uint32_t_create_global ( struct var_collection_uint32_t* collection, struct descriptor_registry_uint32_t* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_uint32_t_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, uint32_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_uint32_t_destroy ( struct var_collection_uint32_t* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint32_t >* > ( collection->internal );
}

void var_collection_uint32_t_for_each ( struct var_collection_uint32_t* collection, void (*func)( const struct var_descriptor*, uint32_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint32_t >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, uint32_t second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_uint32_t_filter ( struct var_collection_uint32_t* target, struct var_collection_uint32_t* collection, int (*func)( const struct var_descriptor*,  uint32_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint32_t >* >( collection->internal );

	var_collection_uint32_t_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint32_t >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, uint32_t second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_uint32_t_concat ( struct var_collection_uint32_t* target, struct var_collection_uint32_t* left, struct var_collection_uint32_t* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint32_t >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint32_t >* >( right->internal );

	var_collection_uint32_t_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint32_t >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
imb::keyed_registry< imb::var_location, uint64_t > * descriptor_registry_uint64_t_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < uint64_t > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_uint64_t_create( struct descriptor_registry_uint64_t* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< uint64_t > ( std::string{name} );

	auto internal = map->find_registry< uint64_t > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_uint64_t_destroy( struct descriptor_registry_uint64_t* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_uint64_t_add_var( struct descriptor_registry_uint64_t* registry, uint64_t value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_uint64_t_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_uint64_t_create ( struct var_collection_uint64_t* collection, struct descriptor_registry_uint64_t* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_uint64_t_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, uint64_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_uint64_t_create_global ( struct var_collection_uint64_t* collection, struct descriptor_registry_uint64_t* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_uint64_t_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, uint64_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_uint64_t_destroy ( struct var_collection_uint64_t* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint64_t >* > ( collection->internal );
}

void var_collection_uint64_t_for_each ( struct var_collection_uint64_t* collection, void (*func)( const struct var_descriptor*, uint64_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint64_t >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, uint64_t second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_uint64_t_filter ( struct var_collection_uint64_t* target, struct var_collection_uint64_t* collection, int (*func)( const struct var_descriptor*,  uint64_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint64_t >* >( collection->internal );

	var_collection_uint64_t_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint64_t >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, uint64_t second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_uint64_t_concat ( struct var_collection_uint64_t* target, struct var_collection_uint64_t* left, struct var_collection_uint64_t* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint64_t >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint64_t >* >( right->internal );

	var_collection_uint64_t_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, uint64_t >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
imb::keyed_registry< imb::var_location, int8_t > * descriptor_registry_int8_t_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < int8_t > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_int8_t_create( struct descriptor_registry_int8_t* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< int8_t > ( std::string{name} );

	auto internal = map->find_registry< int8_t > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_int8_t_destroy( struct descriptor_registry_int8_t* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_int8_t_add_var( struct descriptor_registry_int8_t* registry, int8_t value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_int8_t_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_int8_t_create ( struct var_collection_int8_t* collection, struct descriptor_registry_int8_t* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_int8_t_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, int8_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_int8_t_create_global ( struct var_collection_int8_t* collection, struct descriptor_registry_int8_t* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_int8_t_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, int8_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_int8_t_destroy ( struct var_collection_int8_t* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, int8_t >* > ( collection->internal );
}

void var_collection_int8_t_for_each ( struct var_collection_int8_t* collection, void (*func)( const struct var_descriptor*, int8_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int8_t >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, int8_t second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_int8_t_filter ( struct var_collection_int8_t* target, struct var_collection_int8_t* collection, int (*func)( const struct var_descriptor*,  int8_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int8_t >* >( collection->internal );

	var_collection_int8_t_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int8_t >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, int8_t second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_int8_t_concat ( struct var_collection_int8_t* target, struct var_collection_int8_t* left, struct var_collection_int8_t* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int8_t >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int8_t >* >( right->internal );

	var_collection_int8_t_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int8_t >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
imb::keyed_registry< imb::var_location, int16_t > * descriptor_registry_int16_t_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < int16_t > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_int16_t_create( struct descriptor_registry_int16_t* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< int16_t > ( std::string{name} );

	auto internal = map->find_registry< int16_t > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_int16_t_destroy( struct descriptor_registry_int16_t* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_int16_t_add_var( struct descriptor_registry_int16_t* registry, int16_t value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_int16_t_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_int16_t_create ( struct var_collection_int16_t* collection, struct descriptor_registry_int16_t* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_int16_t_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, int16_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_int16_t_create_global ( struct var_collection_int16_t* collection, struct descriptor_registry_int16_t* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_int16_t_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, int16_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_int16_t_destroy ( struct var_collection_int16_t* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, int16_t >* > ( collection->internal );
}

void var_collection_int16_t_for_each ( struct var_collection_int16_t* collection, void (*func)( const struct var_descriptor*, int16_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int16_t >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, int16_t second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_int16_t_filter ( struct var_collection_int16_t* target, struct var_collection_int16_t* collection, int (*func)( const struct var_descriptor*,  int16_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int16_t >* >( collection->internal );

	var_collection_int16_t_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int16_t >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, int16_t second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_int16_t_concat ( struct var_collection_int16_t* target, struct var_collection_int16_t* left, struct var_collection_int16_t* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int16_t >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int16_t >* >( right->internal );

	var_collection_int16_t_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int16_t >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
imb::keyed_registry< imb::var_location, int32_t > * descriptor_registry_int32_t_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < int32_t > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_int32_t_create( struct descriptor_registry_int32_t* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< int32_t > ( std::string{name} );

	auto internal = map->find_registry< int32_t > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_int32_t_destroy( struct descriptor_registry_int32_t* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_int32_t_add_var( struct descriptor_registry_int32_t* registry, int32_t value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_int32_t_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_int32_t_create ( struct var_collection_int32_t* collection, struct descriptor_registry_int32_t* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_int32_t_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, int32_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_int32_t_create_global ( struct var_collection_int32_t* collection, struct descriptor_registry_int32_t* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_int32_t_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, int32_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_int32_t_destroy ( struct var_collection_int32_t* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, int32_t >* > ( collection->internal );
}

void var_collection_int32_t_for_each ( struct var_collection_int32_t* collection, void (*func)( const struct var_descriptor*, int32_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int32_t >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, int32_t second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_int32_t_filter ( struct var_collection_int32_t* target, struct var_collection_int32_t* collection, int (*func)( const struct var_descriptor*,  int32_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int32_t >* >( collection->internal );

	var_collection_int32_t_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int32_t >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, int32_t second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_int32_t_concat ( struct var_collection_int32_t* target, struct var_collection_int32_t* left, struct var_collection_int32_t* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int32_t >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int32_t >* >( right->internal );

	var_collection_int32_t_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int32_t >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
imb::keyed_registry< imb::var_location, int64_t > * descriptor_registry_int64_t_find_impl( imb::descriptor_registry_map_spec& map, const char* name ){
	auto ptr = map.find_registry < int64_t > ( std::string{name} );
	return ptr;
}

extern "C" {

int descriptor_registry_int64_t_create( struct descriptor_registry_int64_t* registry, const char* name, struct descriptor_registry_ctx* ctx){
	assert(ctx);
	if(!ctx){
		return -1;
	}

	assert(registry);
	if(!registry){
		return -1;
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(ctx->internal);

	map->add_registry< int64_t > ( std::string{name} );

	auto internal = map->find_registry< int64_t > ( std::string{name} );
	if(!internal){
		return -1;
	}

	registry->ctx = ctx;

	size_t name_size = strlen(name);
	char *name_ptr = static_cast<char*>(malloc(sizeof(char)*(name_size+1)));
	strncpy(name_ptr, name, name_size+1);
	
	registry->name = name_ptr;

	return 0;
}

void descriptor_registry_int64_t_destroy( struct descriptor_registry_int64_t* registry ){
	assert(registry);

	if(!registry){
		return;
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return;
	}

	assert(registry->name);
	if(!registry->name){
		return;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	assert(map);
	if(!map){
		return;
	}

	/// @todo implement
	// map->remove(name);

	free(registry->name);
}

size_t descriptor_registry_int64_t_add_var( struct descriptor_registry_int64_t* registry, int64_t value ){
	assert(registry);
	if(!registry){
		return std::numeric_limits<size_t>::max();
	}

	assert(registry->ctx);
	if(!registry->ctx){
		return std::numeric_limits<size_t>::max();
	}
	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);

	auto ptr = descriptor_registry_int64_t_find_impl(*map, registry->name);
	assert(ptr);
	if(!ptr){
		return std::numeric_limits<size_t>::max();
	}

	static size_t i = 0;

	return ptr->add_var(imb::var_location{static_cast<int32_t>(i++), 1,1,1}, value);
}

int var_collection_int64_t_create ( struct var_collection_int64_t* collection, struct descriptor_registry_int64_t* registry ){
	assert( collection && registry && registry->ctx );
	if( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_int64_t_find_impl ( *map, registry->name );

	collection->internal = new (std::nothrow) imb::keyed_var_collection< imb::var_location, int64_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->registry = registry;

	if(! collection->internal ){
		return -1;
	}

	return 0;
}

int var_collection_int64_t_create_global ( struct var_collection_int64_t* collection, struct descriptor_registry_int64_t* registry ) {
	assert( collection && registry && registry->ctx );
	if ( ! ( collection && registry && registry->ctx ) ){
		return -1;
	}

	auto map = reinterpret_cast<imb::descriptor_registry_map_spec*>(registry->ctx->internal);
	auto reg = descriptor_registry_int64_t_find_impl ( *map, registry->name );

	auto var_ptr = new (std::nothrow) imb::keyed_var_collection< imb::var_location, int64_t >(reg->internal_registry(), reg->internal_registry_index());
	collection->internal = var_ptr;
	collection->registry = registry;

	(*var_ptr) = std::move(reg->global_keyed_collection());
	
	if(! collection->internal ){
		return -1;
	}

	return 0;
}

void var_collection_int64_t_destroy ( struct var_collection_int64_t* collection ){
	assert(collection && collection->internal);
	if(! ( collection && collection->internal ) ){
		return;
	}
	delete reinterpret_cast< imb::keyed_var_collection< imb::var_location, int64_t >* > ( collection->internal );
}

void var_collection_int64_t_for_each ( struct var_collection_int64_t* collection, void (*func)( const struct var_descriptor*, int64_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int64_t >* >( collection->internal );

	ptr->for_each([&](const imb::var_location& first, int64_t second){
		struct var_descriptor desc;
		
		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		func(&desc, second);
	});
}

void var_collection_int64_t_filter ( struct var_collection_int64_t* target, struct var_collection_int64_t* collection, int (*func)( const struct var_descriptor*,  int64_t ) ) {
	assert(collection && collection->internal );
	if(! ( collection && collection->internal ) ){
		return;
	}

	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int64_t >* >( collection->internal );

	var_collection_int64_t_create( target, collection->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int64_t >* >( target->internal );
	
	*target_ptr = ptr->filter([&](const imb::var_location& first, int64_t second){
		struct var_descriptor desc;

		desc.name = "";
		desc.jg = first.jg;
		desc.hgrid_id = first.hgrid_id;
		desc.vgrid_id = first.vgrid_id;
		desc.time_id = first.time_id;

		return func(&desc, second) ? 1 : 0;
	});
}

void var_collection_int64_t_concat ( struct var_collection_int64_t* target, struct var_collection_int64_t* left, struct var_collection_int64_t* right){
	assert(left && left->internal );
	if(! ( left && left->internal ) ){
		return;
	}
	assert(right && right->internal );
	if(! ( right && right->internal ) ){
		return;
	}
	assert(right->registry == left->registry);
	if(!(right->registry == left->registry)){
		return;
	}
	assert( target && ! target->internal );
	if(! ( target && ! target->internal ) ){
		return;
	}
	auto left_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int64_t >* >( left->internal );
	auto right_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int64_t >* >( right->internal );

	var_collection_int64_t_create( target, left->registry);
	assert(target->internal);
	if( !target->internal ){
		return;
	}
	auto target_ptr = reinterpret_cast< imb::keyed_var_collection< imb::var_location, int64_t >* >( target->internal );

	*target_ptr = left_ptr->concat(*right_ptr);
}
}
