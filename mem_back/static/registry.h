#pragma once

#include <tuple>
#include <vector>

#include "../var_collection.h"

namespace imb {

template<typename DataDescriptor>
class registry {
public:
	registry(registry<DataDescriptor>&&) = delete;
	registry<DataDescriptor>& operator=(registry<DataDescriptor>&&) = delete;

	registry(const registry&) = delete;
	registry<DataDescriptor>& operator=(const registry<DataDescriptor>&) = delete;

	virtual var_collection<DataDescriptor> global_collection() = 0;

	DataDescriptor& at(size_t i) = 0;

	const DataDescriptor& at(size_t i) = 0;

	size_t add_var(const DataDescriptor& var) = 0;
};

namespace storage {
struct regular {};
}

template<typename DataDescriptor, typename Storage = storage::regular>
class registry_impl;

/**
 * A single registry
 */
template<typename DataDescriptor>
class registry_impl<DataDescriptor, storage::regular> : public registry<DataDescriptor> {
public:
	/**
	 * constructors
	 */
	registry() = default;

	var_collection<DataDescriptor> global_collection() override {
		return var_collection<DataDescriptor>{*this};
	}

	friend class var_collection<DataDescriptor,storage::regular>;

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
private:
	std::vector<DataDescriptor> data;
};

template<typename... DataDescriptors, typename Storage = storage::regular>
class registry_tuple;

/**
 * A tuple of registries for the ease of instantiation
 * This type of class is mostly only useful if you have schemas to be honest
 */
template<typename... DataDescriptors>
class registry_tuple<DataDescriptor..., storage::regular> {
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
	std::tuple<registry<DataDescriptors>...> data;
};

/**
 * Index on a registry based on a key
 */
template<typename Key, typename DataDescriptor>
class registry_index {
public:
	registry_index(registry<DataDescriptor>& data_):
		data{data_}{}

	void add_index(const Key& key, size_t id){
		index.insert({key, id});
	}

	std::optional<size_t> find(const Key& key) const {
		auto find = index.find(key);
		if(find != index.end()){
			return *find;
		}

		return std::nullopt;
	}
private:
	registry<DataDescriptor>& data;
	std::unordered_map<Key, size_t> index;
};
}
