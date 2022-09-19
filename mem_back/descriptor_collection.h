#pragma once

#include "var_collection.h"
#include "var_descriptor.h"

namespace imb {
template<typename T>
class descriptor_var_collection {
public:
	descriptor_var_collection(i_registry<T>& registry_, i_registry_index<var_location, T>& index_):
		collection{registry_, index_}
	{}

	descriptor_var_collection(i_registry<T>& registry_, i_registry_index<var_location, T>& index_, std::vector<std::pair<var_location,size_t>>&& data):
		collection{registry_, index_, std::move(data)}
	{}

	descriptor_var_collection(keyed_var_collection<var_location, T>&& key):
		collection{std::move(key)}
	{}

	descriptor_var_collection(const descriptor_var_collection<T>&) = delete;
	descriptor_var_collection& operator=(const descriptor_var_collection<T>&) = delete;
	
	descriptor_var_collection(descriptor_var_collection<T>&&) = default;
	descriptor_var_collection& operator=(descriptor_var_collection<T>&&) = default;

	template<typename Func>
	void for_each(Func&& func){
		var_descriptor desc;
		desc.name = "foo";
		collection.for_each([f = std::move(func),&desc](const var_location& location, T& val){
			desc.location = location;
			/// @todo 
			f(desc, val);
		});
		//std::move(func));
	}

	descriptor_var_collection clone(){
		auto keyed = collection.clone();

		return {std::move(keyed)};
	}

	template<typename Func>
	descriptor_var_collection sort(Func&& func){
		auto cloned = collection.sort([f = std::move(func)](const var_location, T& val){

		});

		return {std::move(cloned)};
	}
private:
	keyed_var_collection<var_location, T> collection;
};

template<typename... T>
class descriptor_var_collection_variant {
public:
private:

};
}
