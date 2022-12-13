#include "error.h"

namespace imb {
error::error(int64_t ec_, std::variant<std::string, std::string_view> msg_):
	error_code{ec_},
	message_variant{std::move(msg_)}
{
	category_name = error_registry::get_category_name(error_code);
}

std::vector<error_registry::error_category> error_registry::error_categories;

int64_t error_registry::recoverable_next_id = 1;
int64_t error_registry::critical_next_id = -1;

}
