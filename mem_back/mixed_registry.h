#pragma once

#include "./registry.h"

#include <variant>
#include <map>

namespace imb {

/**
 * Is it useful mixing static and non static definitions?
 * Static definitions allow O(1) with constexpr strings
 * At the same time it is possible to add or retrieve things dynamically
 */
template<typename... T>
class mixed_registry_tuple {
public:
private:
	using DynamicRegistry = std::variant<
		registry<float>,
		registry<uint64_t>
	>;

	std::map<std::string,DynamicRegistry> dynamic_registries;

	std::tuple<std::pair<string_literal, registry<T>>...> static_registries;
};

}
