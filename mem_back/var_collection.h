#pragma once

#include <functional>
#include <vector>
#include <map>

#include <cstdint>

namespace imb {

template<typename T>
class registry;

template<typename K, typename T>
class registry_index;

/**
 * Collection of variables
 */
template<typename T>
class var_collection {
public:
	/**
	 * seal type behaviour
	 */
	class builder {
	public:
		builder(registry<T>& registry):
			collection{registry}
		{}

		size_t add_var(const T& var){
			size_t index = collection.registry.add_var(var);
			collection.data.push_back(index);
			return index;
		}

		var_collection<T> build(){
			return std::move(collection);
		}
	private:
		var_collection<T> collection;
	};

	friend class builder;
public:
	/**
	 * constructor
	 */
	var_collection(registry<T>& registry_):
		registry{registry_}
	{}

	var_collection(const var_collection<T>&) = delete;
	var_collection<T>& operator=(const var_collection<T>&) = delete;
	
	var_collection(var_collection<T>&&) = default;
	var_collection<T>& operator=(var_collection<T>&&) = default;

	/**
	 * size of the collection
	 */
	size_t size() const {
		return data.size();
	}

	/**
	 * templated lambda function pass for the iteration
	 */
	template<typename Func>
	void for_each(Func&& func){
		for(auto& iter : data){
			/// get from registry
			auto& var = registry.at(iter);
			func(var);
		}
	}

	/**
	 * clone the current collection
	 */
	var_collection<T> clone(){
		var_collection<T> cloned{registry};
		cloned.data.reserve(data.size());

		std::copy(data.begin(), data.end(), std::back_inserter(cloned.data));

		return cloned;
	}

	/**
	 * function pointer pass for the iteration
	 */
	void for_each(std::function<void(T&)>&& func){
		for(auto& iter : data){
			auto& var = registry.at(iter);
			func(var);
		}
	}

	/**
	 * sort function, which returns a new collection, based on a template function
	 */
	template<typename Func>
	var_collection sort(Func&& func){
		auto cloned = clone();
		std::stable_sort(cloned.data.begin(), cloned.data.end(), [this, sorter = std::move(func)](const size_t& left, const size_t& right){
			return sorter(registry.at(left), registry.at(right));
		});
		return cloned;
	}

	/**
	 * sort function, based on a ptr function
	 */
	var_collection sort(std::function<bool(const T&, const T&)>&& func){
		auto cloned = clone();
		std::stable_sort(cloned.data.begin(), cloned.data.end(), [this, sorter = std::move(func)](const size_t& left, const size_t& right){
			return sorter(registry.at(left), registry.add(right));
		});
		return cloned;
	}

	/**
	 * filter function with a templated functor
	 * creates a new collection which fits the filter criterion
	 */
	template<typename Func>
	var_collection filter(Func&& func){
		var_collection<T> filtered{registry};

		std::copy_if(data.begin(), data.end(), std::back_inserter(filtered.data), [this, filterer = std::move(func)](size_t index){
			return filterer(registry.at(index));
		});

		return filtered;
	}

	/**
	 * filter function with a function pointer
	 * creates a new collection which fits the filter criterion
	 */
	var_collection filter(std::function<bool(const T&)>&& func){
		var_collection<T> filtered{registry};

		std::copy_if(data.begin(), data.end(), std::back_inserter(filtered.data), [this, filterer = std::move(func)](size_t index){
			return filterer(registry.at(index));
		});

		return filtered;
	}
private:
	registry<T>& registry;

	std::vector<size_t> data;
};

template<typename T>
typename var_collection<T>::builder create_collection_builder(registry<T>& registry){
	return {registry};
}

/**
 * I'm not sure if this is necessary since it's not really specified. But this exists for the case
 * that someone wants access to the key type as well
 */
template<typename K, typename T>
class keyed_var_collection {
public:
	class builder {
	public:
		builder(registry<T>& registry_, registry_index<K,T>& index_):
			collection{registry_, index_}
		{}

		keyed_var_collection<K,T> build(){
			return std::move(collection);
		}

		size_t add_var(const K& k, const T& value){
			size_t id = collection.add_var(k,value);
			return id;
		}
	private:
		keyed_var_collection<K,T> collection;
	};

	friend class builder;

	/**
	 * Constructor
	 */
	keyed_var_collection(registry<T>& registry_, registry_index<K,T>& index_):
		registry{registry_},
		index{index_}
	{}

	/**
	 * templated lambda
	 */
	template<typename Func>
	void for_each(Func&& func){
		for(auto& iter : data){
			/// get from registry
			auto& var = registry.at(iter.second);
			func(iter.first, var);
		}
	}

	/**
	 * clone the current collection
	 */
	keyed_var_collection clone(){
		keyed_var_collection<K,T> cloned{registry,index};

		cloned.data.reserve(data.size());

		std::copy(data.begin(), data.end(), std::back_inserter(cloned.data));

		return cloned;
	}

	/**
	 * sort function, which returns a new collection, based on a template function
	 */
	template<typename Func>
	keyed_var_collection sort(Func&& func){
		auto cloned = clone();

		std::stable_sort(cloned.data.begin(), cloned.data.end(), [this, sorter = std::move(func)](const std::pair<K,size_t>& left, const std::pair<K,size_t>& right){
			return sorter({left.first, registry.at(left.second)}, {right.first, registry.at(right.second)});
		});
		return cloned;
	}

	/**
	 * filter function with a templated lambda
	 */
	template<typename Func>
	keyed_var_collection filter(Func&& func){
		keyed_var_collection<K,T> filtered{registry,index};

		std::copy_if(data.begin(), data.end(), std::back_inserter(filtered.data), [this, filterer = std::move(func)](std::pair<K, size_t> ind){
			return filterer({ind.first, registry.at(ind.second)});
		});

		return filtered;
	}
	
private:
	registry<T>& registry;
	registry_index<K, T>& index;

	std::vector<std::pair<K,size_t>> data;

	size_t add_var(const K& key, const T& value){
		size_t index_id = registry.add_var(value);
		index.add_index(key, index_id);

		data.push_back({key, index_id});

		return index_id;
	}
};

template<typename K, typename T>
typename keyed_var_collection<K, T>::builder create_keyed_collection_builder(registry<T>& registry_, registry_index<K,T>& registry_index_){
	return {registry_, registry_index_};
}

template<typename K, typename T>
class keyed_registry;

template<typename K, typename T>
typename keyed_var_collection<K, T>::builder create_keyed_collection_builder(keyed_registry<K,T>& registry_){
	return {registry_.registry, registry_.index};
}
}
