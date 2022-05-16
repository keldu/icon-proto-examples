#pragma once

#include <string>

#include <cstdint>

namespace imb {
class var_descriptor {
private:
	std::string name;
	int32_t jg;
	int32_t hgrid_id;
	int32_t vgrid_id;
	int32_t time_id;
};
}
