#pragma once

#include <cstdint>
#include <variant>
#include <vector>
#include <string>
#include <string_view>

#include <iostream>

namespace imb {
class error {
private:
	int64_t error_code;
	std::string_view category_name;

	std::variant<std::string, std::string_view> message_variant;
public:
	error(int64_t ec_, std::string_view cat_name_, std::variant<std::string, std::string_view> msg_);

	int64_t code() const {
		return error_code;
	}

	std::string_view name() const {
		return category_name;
	}

	std::string_view message() const {
		return std::visit([](auto& arg) -> std::string_view{
			return arg;
		}, message_variant);
	}

	template<typename T>
	bool is_type() const;

};


template<typename T>
int64_t& get_id_helper(){
	static int64_t id = 0;

	return id;
}

class error_registry {
private:
	static int64_t recoverable_next_id;
	static int64_t critical_next_id;

	struct error_category {
		int64_t id;
		std::string name;
	};
	static std::vector<error_category> error_categories;
public:
	static int64_t new_error_type(std::string_view name, bool is_critical);

	template<typename T>
	static int64_t get_error_id(){
		return get_id_helper<T>();
	}

	/**
	 * Setup which allows for the use of typed templates
	 */
	template<typename T>
	static int64_t get_or_new_error_id(std::string_view name, bool is_critical){
		int64_t& id_helper = get_id_helper<T>();
		if(id_helper != 0){
			return get_id_helper<T>();
		}
		int64_t id = [&](){
			int64_t id = 0;
			if(is_critical){
				id = critical_next_id--;
			}else{
				id = recoverable_next_id++;
			}
			return id;
		}();
		id_helper = id;

		error_categories.emplace_back(error_category{id, std::string{name}});

		return id;
	}

	/**
	 * check if the registered type is the corresponding to the provided id
	 */
	template <typename T>
	static bool is_error_type(int64_t id) {
		return (id == get_id_helper<T>());
	}

	static std::string_view get_category_name(int64_t id) {
		for(auto& iter : error_categories){
			if( iter.id == id ){
				return iter.name;
			}
		}

		return "Category not found";
	}
};

template< typename T >
bool error::is_type() const {
	return error_registry::template is_error_type<T>(error_code);
}

template <typename T>
error make_error(const std::string& msg){
	auto id = error_registry::get_or_new_error_id<T>(T::name, T::is_critical);
	
	return error { id, T::name, msg };
}

template<typename T>
class error_or {
private:
	/**
	 * Always an error or a value
	 */
	std::variant<error,T> error_or_value_variant;
public:
	/**
	 * Construct an error state
	 */
	error_or(const error& error):
		error_or_value_variant{error}
	{}

	/**
	 * Construct a value
	 */
	error_or(T&& value):
		error_or_value_variant{std::move(value)}
	{}

	/**
	 * Returns true if state is in an error state
	 */
	bool is_error() const {
		return std::holds_alternative<error>(error_or_value_variant);
	}

	/**
	 * Returns true if state is in a value state
	 */
	bool is_value() const {
		return std::holds_alternative<T>(error_or_value_variant);
	}

	/**
	 * Returns an error. Assumes you checked this with is_error
	 */
	error& get_error() {
		return std::get<error>(error_or_value_variant);
	}

	/**
	 * Returns a value. Assumes you checked this with is_value
	 */
	T& get_value() {
		return std::get<T>(error_or_value_variant);
	}
};


}

namespace std {
std::ostream& operator<<(std::ostream& stream, const imb::error& err);
/*
{
	std::cout<<err.name()<<" "<<err.code()<<" "<<err.message();
	return stream;
}
*/
}
