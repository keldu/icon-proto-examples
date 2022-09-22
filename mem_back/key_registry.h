#pragma once

#include "static/registry.h"

#include <string>
#include <variant>
#include <map>
#include <memory>

namespace imb {

template<typename Key, typename DataDescriptor>
class i_keyed_registry {
public:
	i_keyed_registry() = default;
	
	i_keyed_registry(i_keyed_registry<Key,DataDescriptor>&&) = delete;
	i_keyed_registry<Key, DataDescriptor>& operator=(i_keyed_registry<Key,DataDescriptor>&&) = delete;
	
	i_keyed_registry(const i_keyed_registry<Key,DataDescriptor>&) = delete;
	i_keyed_registry<Key, DataDescriptor>& operator=(const i_keyed_registry<Key,DataDescriptor>&) = delete;
	
	virtual size_t add_var(const Key& key, DataDescriptor&& var) = 0;
	virtual size_t add_var(const Key& key, const DataDescriptor& var) = 0;
	
	virtual std::optional<size_t> find_id(const Key& key) const = 0;
	
	virtual const DataDescriptor* find(const Key& key) const = 0;
	
	virtual DataDescriptor* find(const Key& key) = 0;
	
	virtual const DataDescriptor& at(size_t id) const = 0;
	
	virtual DataDescriptor& at(size_t id) = 0;

	virtual i_registry<DataDescriptor>& internal_registry() = 0;

	virtual i_registry_index<Key, DataDescriptor>& internal_registry_index() = 0;
protected:
	~i_keyed_registry() = default;

	friend class keyed_var_collection<Key,DataDescriptor>;
};

template<typename Key, typename DataDescriptor, typename Storage = storage::regular, typename IndexStorage = storage::regular>
class keyed_registry : public i_keyed_registry<Key,DataDescriptor>{
public:
	keyed_registry():
		registry_table{},
		index{registry_table}
	{}
	
	var_collection<DataDescriptor> global_collection() {
		return var_collection<DataDescriptor>{registry_table};
	}

	keyed_var_collection<Key, DataDescriptor> global_keyed_collection() {
		std::vector<std::pair<Key,size_t>> data;
		index.for_each([&data](const std::pair<Key, size_t>& pair){
			data.emplace_back(pair);
		});

		return keyed_var_collection<Key, DataDescriptor>{registry_table, index, std::move(data)};
	}

	/**
	 * Add a variable to the registry itself
	 */
	size_t add_var(const Key& key, DataDescriptor&& var) override {
		size_t id = registry_table.add_var(std::move(var));
		
		index.add_index(key, id);

		return id;
	}

	size_t add_var(const Key& key, const DataDescriptor& var) override {
		size_t id = registry_table.add_var(var);

		index.add_index(key, id);

		return id;
	}

	/**
	 * One might try to find ids based on the keys and use that.
	 */
	std::optional<size_t> find_id(const Key& key) const override {
		std::optional<size_t> id = index.find(key);

		return id;
	}

	const DataDescriptor* find(const Key& key) const override {
		std::optional<size_t> id = index.find(key);
		if(!id.has_value()){
			return nullptr;
		}

		auto& data = registry_table.at(*id);

		return &data;
	}

	DataDescriptor* find(const Key& key) override {
		std::optional<size_t> id = index.find(key);
		if(!id.has_value()){
			return nullptr;
		}
		
		auto& data = registry_table.at(*id);

		return &data;
	}

	const DataDescriptor& at(size_t id) const override {
		return registry_table.at(id);
	}

	DataDescriptor& at(size_t id) override {
		return registry_table.at(id);
	}
	
	i_registry<DataDescriptor>& internal_registry() override {
		return registry_table;
	};

	i_registry_index<Key, DataDescriptor>& internal_registry_index() override{
		return index;
	}
public:
	/**
	 * Shouldn't be public
	 */
	registry<DataDescriptor, Storage> registry_table;
	registry_index<Key,DataDescriptor, IndexStorage> index;
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

/**
 * Dynamic variant of the registry tuple
 * @todo add storage definitions
 */
template<typename Key, typename... DataDesc>
class keyed_registry_map {
public:
	using keyed_registry_variant = std::variant<keyed_registry<Key,DataDesc>...>;
	using keyed_registry_variant_ptr = std::variant<std::monostate,keyed_registry<Key,DataDesc>*...>;

	keyed_registry_variant_ptr find_registry_variant(const std::string& name){
		auto find = mapped_registries.find(name);

		if(find != mapped_registries.end()){
			return std::visit(
				[](auto&& arg) -> keyed_registry_variant_ptr{
					return &arg;
				}
			, *(find->second));
		}

		return std::monostate{};
	}

	template<typename D>
	keyed_registry<Key,D>* find_registry(const std::string& name){
		keyed_registry_variant_ptr reg_var = find_registry_variant(name);
		if(std::holds_alternative<std::monostate>(reg_var)){
			return nullptr;
		}

		return std::get<keyed_registry<Key,D>*>(reg_var);
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

	void remove_registry(const std::string& name) {
		mapped_registries.erase(name);
	}
private:
	std::map<std::string, std::unique_ptr<keyed_registry_variant>> mapped_registries;
};
}
