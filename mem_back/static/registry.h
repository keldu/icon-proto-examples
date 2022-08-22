#pragma once

#include "../var_collection.h"

#include <tuple>
#include <vector>
#include <optional>

namespace imb {

/**
 * Since plugins are able to work with virtual classes, the fortran interface should be able to as well.
 * afaik instantiation and deletion should happen in the plugins/c++ part.  
 * E.g. deallocator with a custom deletion function for std::unique_ptr in case of plugins.
 *
 * This only exists just so var_collection doesn't have to have an individual type for each backend storage
 */
template<typename DataDescriptor>
class i_registry {
public:
	i_registry() = default;

	i_registry(i_registry<DataDescriptor>&&) = delete;
	i_registry<DataDescriptor>& operator=(i_registry<DataDescriptor>&&) = delete;

	i_registry(const i_registry&) = delete;
	i_registry<DataDescriptor>& operator=(const i_registry<DataDescriptor>&) = delete;

	virtual var_collection<DataDescriptor> global_collection() = 0;

	virtual DataDescriptor& at(size_t i) = 0;

	virtual const DataDescriptor& at(size_t i) const = 0;

	virtual size_t add_var(const DataDescriptor& var) = 0;

	virtual void sync() = 0;
protected:
	~i_registry() = default;
	
	friend class var_collection<DataDescriptor>;
};

/**
 * Idea on how to change
 */
template<typename DataDescriptor>
class registry_var {
private:
	DataDescriptor var;
};

namespace storage {
struct regular {};
}

template<typename DataDescriptor, typename Storage = storage::regular>
class registry;

/**
 * A single registry
 */
template<typename DataDescriptor>
class registry<DataDescriptor, storage::regular> : public i_registry<DataDescriptor> {
public:
	/**
	 * constructors
	 */
	registry() = default;

	var_collection<DataDescriptor> global_collection() override {
		return var_collection<DataDescriptor>{*this};
	}

	/**
	 * get data at index
	 */
	DataDescriptor& at(size_t i) override {
		return data.at(i);
	}

	/**
	 * get data at index
	 */
	const DataDescriptor& at(size_t i) const override {
		return data.at(i);
	}

	/**
	 * add data
	 */
	size_t add_var(const DataDescriptor& var) override {
		size_t index = data.size();
		data.push_back(var);
		return index;
	}

	void sync() override {
		/// Do nothing. No sync has to happen
	}
private:
	std::vector<DataDescriptor> data;
};

template<typename Storage = storage::regular,typename... DataDescriptors>
class registry_tuple;

/**
 * A tuple of registries for the ease of instantiation
 * This type of class is mostly only useful if you have schemas to be honest
 */
template<typename... DataDescriptors>
class registry_tuple<storage::regular,DataDescriptors...> {
public:
	template<typename Descriptor>
	var_collection<Descriptor> global_collection(){
		return std::get<registry<Descriptor>>(data).global_collection();
	}

	template<typename Descriptor>
	registry<Descriptor>& get_registry(){
		return std::get<registry<Descriptor>>(data);
	}
private:
	std::tuple<registry<DataDescriptors, storage::regular>...> data;
};

template<typename Key, typename DataDescriptor>
class i_registry_index {
public:
	virtual void add_index(const Key& key, size_t id) = 0;
	
	virtual std::optional<size_t> find(const Key& key) const = 0;
};

template<typename Key, typename DataDescriptor, typename Storage = storage::regular>
class registry_index;

/**
 * Index on a registry based on a key
 */
template<typename Key, typename DataDescriptor>
class registry_index<Key, DataDescriptor, storage::regular> : public i_registry_index<Key,DataDescriptor> {
public:
	// This could be any type though
	registry_index(registry<DataDescriptor, storage::regular>& data_):
		data{data_}{}

	void add_index(const Key& key, size_t id) override {
		index.insert({key, id});
	}

	std::optional<size_t> find(const Key& key) const override {
		auto find = index.find(key);
		if(find != index.end()){
			return find->second;
		}

		return std::nullopt;
	}

	template<typename Func>
	void for_each(Func&& func){
		for(auto& iter : index){
			func(iter);
		}
	}
private:
	registry<DataDescriptor,storage::regular>& data;
	std::unordered_map<Key, size_t> index;
};
}
