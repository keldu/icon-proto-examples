#pragma once

#include "descriptor_registry.h"

namespace imb {
using descriptor_registry_map_spec = 
	descriptor_registry_map<
		float,
		double,
		uint8_t,
		uint16_t,
		uint32_t,
		uint64_t,
		int8_t,
		int16_t,
		int32_t,
		int64_t
	>;
}
