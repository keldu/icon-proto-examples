#pragma once

#include "var_data.h"
#include "var_descriptor.h"
#include "var_meta_data.h"

#include <memory>

namespace imb {
class var {
private:
	var_descriptor descriptor;
	std::unique_ptr<var_meta_data> meta_data;
	std::unique_ptr<var_data> data;
};
}
