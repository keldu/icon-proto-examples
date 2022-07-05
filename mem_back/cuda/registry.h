#pragma once

#include "../static/registry.h"

namespace imb {
namespace storage {
struct cuda {};
}
template<typename DataDescriptor>
class registry_impl<DataDescriptor,storage::cuda> : public registry<DataDescriptor>{
public:
	registry() = default;


	var_collection<DataDescriptor, storage::cuda> global_collection() {
		
	}
};
}
