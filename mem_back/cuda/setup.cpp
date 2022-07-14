#include "./registry.h"

using cuda_registry = imb::registry_tuple<imb::storage::cuda, uint64_t>;

int main(){
	using namespace imb;

	cuda_registry cuda_reg;

	return 0;
}
