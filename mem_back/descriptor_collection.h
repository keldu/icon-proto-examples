#pragma once

#include "var_collection.h"
#include "var_descriptor.h"

#include <string>

namespace imb {
template<typename... T>
class descriptor_registry_map;
template<typename T>
class descriptor_var_collection {
public:
	descriptor_var_collection(i_registry<T>& registry_, i_registry_index<var_location, T>& index_, const std::string& name):
		collection{registry_, index_},
		name{name}
	{}

	descriptor_var_collection(i_registry<T>& registry_, i_registry_index<var_location, T>& index_, const std::string& name, std::vector<std::pair<var_location,size_t>>&& data):
		collection{registry_, index_, std::move(data)},
		name{name}
	{}

	descriptor_var_collection(keyed_var_collection<var_location, T>&& key, const std::string& name):
		collection{std::move(key)},
		name{name}
	{}

	descriptor_var_collection(const descriptor_var_collection<T>&) = delete;
	descriptor_var_collection& operator=(const descriptor_var_collection<T>&) = delete;
	
	descriptor_var_collection(descriptor_var_collection<T>&&) = default;
	descriptor_var_collection& operator=(descriptor_var_collection<T>&&) = default;

	template<typename Func>
	void for_each(Func&& func){
		var_descriptor desc;
		desc.name = name;
		collection.for_each([f = std::move(func),&desc](const var_location& location, T& val){
			desc.location = location;
			f(desc, val);
		});
	}

	descriptor_var_collection clone(){
		auto keyed = collection.clone();

		return {std::move(keyed), name};
	}

	template<typename Func>
	descriptor_var_collection sort(Func&& func){
		auto cloned = collection.sort([f = std::move(func)](const var_location, T& val){

		});

		return {std::move(cloned), name};
	}
private:
	keyed_var_collection<var_location, T> collection;
	std::string name;
};

template<typename... T>
class descriptor_var_collection_variant {
public:
private:

};
}
