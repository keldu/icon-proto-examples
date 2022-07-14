#include "./registry.h"

using cuda_registry = imb::registry<uint64_t,imb::storage::cuda>;

int main(){
	using namespace imb;

	cuda_registry cuda_reg;

	return 0;
}
