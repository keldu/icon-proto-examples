#pragma once

#include "registry.h"

namespace imb {

template<typename Key, typename DataDescriptor>
class keyed_registry {
public:
	keyed_registry():
		registry{},
		index{registry}
	{}

	var_collection<DataDescriptor> global_collection() {
		return var_collection<DataDescriptor>{registry};
	}

	keyed_registry(keyed_registry<Key,DataDescriptor>&&) = delete;
	keyed_registry<Key, DataDescriptor>& operator=(keyed_registry<Key,DataDescriptor>&&) = delete;
	
	keyed_registry(const keyed_registry<Key,DataDescriptor>&) = delete;
	keyed_registry<Key, DataDescriptor>& operator=(const keyed_registry<Key,DataDescriptor>&) = delete;

	

	/**
	 * Add a variable to the registry itself
	 */
	size_t add_var(const Key& key, DataDescriptor& var){
		size_t id = registry.add_var(var);
		
		index.add_index(key, id);

		return id;
	}

	/**
	 * One might try to find ids based on the keys and use that.
	 */
	std::optional<size_t> find_id(const Key& key) const {
		std::optional<size_t> id = index.find(key);

		return id;
	}

	const DataDescriptor* find(const Key& key) const {
		std::optional<size_t> id = index.find(key);
		if(!id.has_value()){
			return nullptr;
		}

		auto& data = registry.at(*id);

		return &data;
	}

	DataDescriptor* find(const Key& key){
		std::optional<size_t> id = index.find(key);
		if(!id.has_value()){
			return nullptr;
		}
		
		auto& data = registry.at(*id);

		return &data;
	}

	const DataDescriptor& at(size_t id) const {
		return  registry.at(id);
	}

	DataDescriptor& at(size_t id) {
		return  registry.at(id);
	}

	//
private:
	registry<DataDescriptor> registry;
	registry_index<Key,DataDescriptor> index;

	friend class keyed_collection_builder<Key,DataDescriptor>;
};

/**
 * minor template helper
 */
template<typename... T>
constexpr bool always_false = false;

/**
 * key registry tuple
 */
template<typename... T>
class keyed_registry_tuple{
	/**
	 * Sadly this is necessary, because a simple false would trigger the assert even when no instantiation of this general case is created
	 */
	static_assert(always_false<T...>, "Template case not supported");
};

template<typename Key, typename Value>
struct kv {};

/**
 * Currently not that necessary
 * @unimplemented
 */
template<typename... Keys, typename... DataDesc>
class keyed_registry_tuple<kv<Keys, DataDesc>...> {
public:
private:
	std::tuple<keyed_registry<Keys, DataDesc>...> registries;
};

/**
 * Currently not that necessary
 * @unimplemented
 */
template<typename Key, typename... DataDesc>
class keyed_registry_tuple<Key, DataDesc...> {
public:
private:
	std::tuple<keyed_registry<Key, DataDesc>...> registries;
};
}
