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
		std::pair<var_descriptor,T> left_p;
		std::pair<var_descriptor,T> right_p;
		left_p.first.name = name;
		right_p.first.name = name;
		auto cloned = collection.sort([f = std::move(func), &left_p, &right_p](const std::pair<var_location,T>& left, const std::pair<var_location,T>& right){
			left_p.first.location = left.first;
			left_p.second = left.second;
			
			right_p.first.location = right.first;
			right_p.second = left.second;
			return f(left_p, right_p);
		});

		return {std::move(cloned), name};
	}

	template<typename Func>
	descriptor_var_collection filter(Func&& func){
		var_descriptor desc;
		desc.name = name;
		auto cloned = collection.filter([f = std::move(func), &desc](const var_location& location, const T& val){
			desc.location = location;
			return f(desc, val);
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
