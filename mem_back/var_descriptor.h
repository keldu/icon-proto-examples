#pragma once

#include <string>

#include <cstdint>

namespace imb {
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
private:
	std::string name;
	int32_t jg;
	int32_t hgrid_id;
	int32_t vgrid_id;
	int32_t time_id;
};
}
