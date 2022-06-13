#include "key_registry.h"

#include <string>

namespace imb {

}

int main(){
	imb::keyed_registry<uint32_t, std::string> registry;

	/// Builder
	auto builder = create_keyed_collection_builder<uint32_t, std::string>(registry.registry, registry.index);

	/// element isn't necessary, but provides the created index
	size_t element{};

	auto collection = builder.build();

	collection.for_each([](size_t value){
		(void) value;
	});

	return 0;
}
