#pragma once

#include <array>

#include <cstdint>

namespace imb {
class shape_info {
private:
	int32_t itype;
	std::array<int32_t, 7> idim;

};
}
