#pragma once

#include <tuple>
#include <vector>

#include "../var_collection.h"

namespace imb {

/**
 * A single registry
 */
template<typename DataDescriptor>
class registry {
public:
	/**
	 * constructors
	 */
	registry() = default;

	registry(registry<DataDescriptor>&&) = delete;
	registry<DataDescriptor>& operator=(registry<DataDescriptor>&&) = delete;

	registry(const registry&) = delete;
	registry<DataDescriptor>& operator=(const registry<DataDescriptor>&) = delete;

	var_collection<DataDescriptor> global_collection() {
		return var_collection<DataDescriptor>{*this};
	}

	friend class var_collection<DataDescriptor>;

	/**
	 * get data at index
	 */
	DataDescriptor& at(size_t i){
		return data.at(i);
	}

	/**
	 * get data at index
	 */
	const DataDescriptor& at(size_t i) const {
		return data.at(i);
	}

	/**
	 * add data
	 */
	size_t add_var(const DataDescriptor& var){
		size_t index = data.size();
		data.push_back(var);
		return index;
	}
private:
	std::vector<DataDescriptor> data;
};

/**
 * A tuple of registries for the ease of instantiation
 */
template<typename... DataDescriptors>
class registry_tuple {
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
