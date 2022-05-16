#pragma once

#include <tuple>
#include <vector>

namespace imb {

template<typename... DataDescriptor>
class registry {
public:
private:
	std::vector<std::tuple<DataDescriptor....>> data;
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
