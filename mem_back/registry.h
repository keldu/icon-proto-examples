#pragma once

#include <tuple>
#include <vector>

#include "var_collection.h"

namespace imb {

template<typename... DataDescriptor>
class registry {
public:
	template<typename Desc>
	var_collection<Desc> global_collection() const;


private:
	std::tuple<std::vector<DataDescriptor>...> data;
};

template<typename Key, typename... DataDescriptor>
class registry_index {
public:
	registry_index(registry<DataDescriptor...>& data_):
		data{data_}{}


private:
	registry<DataDescriptor...>& data;
	std::unordered_map<Key, size_t> index;
};
}
