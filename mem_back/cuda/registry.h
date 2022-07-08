#pragma once

#include "../static/registry.h"



namespace imb {
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

	}
public:
	struct allocator {
		using value_type = DataDescriptor;

		value_type* allocate(size_t len){
			void* ptr{};

			cudaMalloc(&ptr, len);

			return ptr;
		}

		void deallocate(value_type* buffer, size_t len){
			if(!buffer){
				return;
			}

			cu
		}
	};
private:

	std::vector<DataDescriptor,allocator> data;
};

template<typename Key, typename DataDescriptor>
class registry_index<Key,DataDescriptor, storage::cuda> : public i_registry_index<Key, DataDescriptor> {
public:
};
}
