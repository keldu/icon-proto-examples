#pragma once

#include "registry.h"

#include <string>
#include <variant>
#include <map>
#include <memory>

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
	size_t add_var(const Key& key, DataDescriptor&& var){
		size_t id = registry.add_var(std::move(var));
		
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
public:
	/**
	 * Shouldn't be public
	 */
	registry<DataDescriptor> registry;
	registry_index<Key,DataDescriptor> index;

	friend class keyed_var_collection<Key,DataDescriptor>;
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
	static_assert(always_false<T...>, "Template not supported");
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

template<typename Key, typename... DataDesc>
class keyed_registry_map {
public:
	using keyed_registry_variant = std::variant<keyed_registry<Key,DataDesc>...>;

	keyed_registry_variant* find_registry_variant(const std::string& name){
		auto find = mapped_registries.find(name);

		if(find != mapped_registries.end()){
			return find->second.get();
		}

		return nullptr;
	}

	template<typename D>
	keyed_registry<Key,D>* find_registry(const std::string& name){
		keyed_registry_variant* reg_var = find_registry_variant(name);
		if(!reg_var){
			return nullptr;
		}

		return &std::get<keyed_registry<Key,D>>(*reg_var);
	}

	template<typename D>
	bool add_registry(const std::string& key_name){
		auto ptr = std::make_unique<keyed_registry_variant>();
		ptr->template emplace<keyed_registry<Key,D>>();

		auto inserted = mapped_registries.emplace(std::make_pair(
			key_name, std::move(ptr)
		));
		
		return inserted.second;
	}
private:
	std::map<std::string, std::unique_ptr<keyed_registry_variant>> mapped_registries;
};
}
