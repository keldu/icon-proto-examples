#pragma once

#include <functional>
#include <vector>

#include <cstdint>

namespace imb {

/**
 * Collection of variables
 */
template<typename T>
class var_collection {
public:
	class builder {
	public:
		void add_var();
	private:
		var_collection collection;
	};

public:
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
			func(iter);
		}
	}

	/**
	 * function pointer pass for the iteration
	 */
	void for_each(std::function<void(T&)>&& func){
		for(auto& iter : data){
			func(iter);
		}
	}

	/**
	 * sort function, which returns a new collection, based on a template function
	 */
	template<typename Func>
	var_collection sort(Func&& func);

	/**
	 * sort function, based on a ptr function
	 */
	var_collection sort(std::function<bool(const T&, const T&)>&& func);

	/**
	 * filter function with a templated functor
	 * creates a new collection which fits the filter criterion
	 */
	template<typename Func>
	var_collection filter(Func&& func);

	/**
	 * filter function with a function pointer
	 * creates a new collection which fits the filter criterion
	 */
	var_collection filter(std::function<bool(const T&)>&& func);
private:
	std::vector<size_t> data;

};
}
