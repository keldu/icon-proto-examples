#pragma once

#include "../static/registry.h"

#include <cuda.h>
#include <cuda_runtime.h>

#include <array>
#include <vector>
#include <iostream>

namespace imb {
namespace storage {
struct cuda_managed {};
}

template<typename T>
struct cuda_managed_allocator {
	using value_type = T;

	value_type* allocate(size_t len){
		void* buffer{};

		auto rc = cudaMallocManaged(&buffer, len);
		if(rc != cudaSuccess){
			std::cerr<<"cudaMallocManaged: "<<cudaGetErrorString(error)<<std::endl;
			exit(-1);
		}

		return reinterpret_cast<value_type*>(buffer);
	}

	void deallocate(value_type* buffer, size_t len){
		if(!buffer){
			return;
		}

		// We don't need this
		(void) len;

		cudaFree(buffer);
	}
};

template<typename DataDescriptor>
class registry<DataDescriptor,storage::cuda_managed> : public i_registry<DataDescriptor>{
public:
	registry() = default;

	var_collection<DataDescriptor> global_collection() override {
		return {*this};
	}

	DataDescriptor& at(size_t i) override {
		return data.at(i);
	}

	const DataDescriptor& at(size_t i) const override {
		return data.at(i);
	}

	size_t add_var(const DataDescriptor& var) override {
		size_t index = data.size();
		data.push_back(var);

		return index;
	}

	void sync() override {

	}
private:

	std::vector<DataDescriptor,cuda_managed_allocator<DataDescriptor>> data;
};

template<typename Key, typename DataDescriptor>
class registry_index<Key,DataDescriptor, storage::cuda_managed> : public i_registry_index<Key, DataDescriptor> {
public:
	registry_index(registry<DataDescriptor, storage::cuda_managed>& data_):
		data{data_}
	{}

	void add_index(const Key& key, size_t id) override {
		index.push_back(std::make_pair(key,id));
	}

	std::optional<size_t> find(const Key& key) const override {
		auto find = std::find_if(index.begin(), index.end(), [&key](const std::pair<Key, size_t>& iter){
			return iter.first == key;
		});

		if(find != index.end()){
			return find->second;
		}

		return std::nullopt;
	}
private:
	registry<DataDescriptor, storage::cuda_managed>& data;
	std::vector<std::pair<Key, size_t>, cuda_managed_allocator<DataDescriptor>> index;
};

namespace storage {
struct cuda {};
}

template<typename DataDescriptor>
class registry<DataDescriptor,storage::cuda> : public i_registry<DataDescriptor>{
public:
	registry() = default;

	var_collection<DataDescriptor> global_collection() override {
		return {*this};
	}

	DataDescriptor& at(size_t i) override {
		if( !mini_cache.has_value() ){
			
		}
		if( i == mini_cache.index ){
			return mini_cache
		}
		return data.at(i);
	}

	const DataDescriptor& at(size_t i) const override {
		return data.at(i);
	}

	size_t add_var(const DataDescriptor& var) override {
		size_t index = data.size();

		data.push_back(var);

		return index;
	}

	void sync() override {

	}
private:
	struct cuda_vector {
		size_t size = 0;
		void* data = nullptr;
	};
	cuda_vector vec;

	struct mini_cache {
		DataDescriptor data;
		size_t index;
	};
	std::optional<mini_cache> mini_cache = std::nullopt;

};

template<typename Key, typename DataDescriptor>
class registry_index<Key,DataDescriptor, storage::cuda> : public i_registry_index<Key, DataDescriptor> {
public:
	registry_index(registry<DataDescriptor, storage::cuda_managed>& data_):
		data{data_}
	{}

	void add_index(const Key& key, size_t id) override {
		index.push_back(std::make_pair(key,id));
	}

	std::optional<size_t> find(const Key& key) const override {
		auto find = std::find_if(index.begin(), index.end(), [&key](const std::pair<Key, size_t>& iter){
			return iter.first == key;
		});

		if(find != index.end()){
			return find->second;
		}

		return std::nullopt;
	}
private:
	registry<DataDescriptor, storage::cuda_managed>& data;
	std::vector<std::pair<Key, size_t>, cuda_managed_allocator<DataDescriptor>> index;
};
}
