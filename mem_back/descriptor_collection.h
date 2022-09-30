#pragma once

#include "var_collection.h"
#include "var_descriptor.h"

#include <string>
#include <variant>

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

	size_t size() const {
		return collection.size();
	}
private:
	keyed_var_collection<var_location, T> collection;
	std::string name;
};

template<typename... T>
class descriptor_var_collection_variant {
public:
	struct data {
		size_t id;
		var_descriptor key;
	};
public:
	descriptor_var_collection_variant(descriptor_registry_map<T...>& maps_):
		maps{&maps_}
	{}

	descriptor_var_collection_variant(descriptor_registry_map<T...>& maps_, std::vector<descriptor_var_collection_variant::data>&& data_):
		maps{&maps_},
		data{std::move(data_)}
	{}
	
	descriptor_var_collection_variant(const descriptor_var_collection_variant<T...>&) = delete;
	descriptor_var_collection_variant& operator=(const descriptor_var_collection_variant<T...>&) = delete;
	
	descriptor_var_collection_variant(descriptor_var_collection_variant<T...>&&) = default;
	descriptor_var_collection_variant& operator=(descriptor_var_collection_variant<T...>&&) = default;

	descriptor_var_collection_variant clone(){
		descriptor_var_collection_variant<T...> cloned{*maps};
		cloned.data.reserve(data.size());

		std::copy(data.begin(), data.end(), std::back_inserter(cloned.data));

		return cloned;
	}
	
	template<typename Func>
	void for_each(Func&& func){
		for(auto& iter: data){
			auto reg = maps->find_registry_variant(iter.name);
			if(reg){
				auto var = std::visit([&iter](auto& arg) -> std::variant<T*...> {
					auto& val = arg.at(iter.id);
					return &val;
				}
				, *reg);
				
				func(iter.key, var);
			}
		}
	}
private:
	descriptor_registry_map<T...>* maps;
	std::vector<data> data;
};
}
