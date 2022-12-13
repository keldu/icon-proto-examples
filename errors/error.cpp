#include "error.h"

namespace imb {
error::error(int64_t ec_, std::string_view cat_name_, std::variant<std::string, std::string_view> msg_):
	error_code{ec_},
	category_name{cat_name_},
	message_variant{std::move(msg_)}
{
}

std::vector<error_registry::error_category> error_registry::error_categories;

int64_t error_registry::recoverable_next_id = 1;
int64_t error_registry::critical_next_id = -1;

}
namespace std {
std::ostream& operator<<(std::ostream& stream, const imb::error& err)
{
	std::cout<<err.name()<<" "<<err.code()<<" "<<err.message();
	return stream;
}
}
