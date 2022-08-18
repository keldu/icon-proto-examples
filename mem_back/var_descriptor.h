#pragma once

#include <string>

#include <cstdint>

namespace imb {

class var_location {
public:
	int32_t jg;
	int32_t hgrid_id;
	int32_t vgrid_id;
	int32_t time_id;

	bool operator==(const var_location& rhs) const {
		return jg == rhs.jg && hgrid_id == rhs.hgrid_id && vgrid_id == rhs.vgrid_id && time_id;
	}
};

/**
 * Personally I don't like the string here.
 * The only thing that describes a type is the string.
 * As far as I can see the other ids just describe positional data.
 * So I think it should be sorted out earlier.
 *
 * For example by grabbing the registry with the string and then accessing the remaining part with the ids
 * Should be faster as well.
 */
class var_descriptor {
public:
	std::string name;
	var_location location;
	
};

}
namespace std {

template<>
struct hash<imb::var_location> {
	template<typename T>
	static void hash_combine(std::size_t& seed, const T& v){

		seed ^= std::hash<T>{}(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
	}

	std::size_t operator()(const imb::var_location& k) const {
		using std::size_t;

		size_t seed = 0;
		size_t hash{};
		hash_combine<int32_t>(seed, k.jg);
		hash_combine<int32_t>(seed, k.hgrid_id);
		hash_combine<int32_t>(seed, k.vgrid_id);
		hash_combine<int32_t>(seed, k.time_id);

		return seed;
	}
};

}
