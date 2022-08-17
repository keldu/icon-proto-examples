#pragma once

#include "key_registry.h"
#include "var_descriptor.h"

#include <variant>
#include <optional>

namespace imb {

/**
 * The registry which
 */ 
template<typename... T>
class descriptor_registry_map {
public:
	using keyed_registry_variant = typename keyed_registry_map<var_location,T...>::keyed_registry_variant;
	using keyed_registry_variant_ptr = typename keyed_registry_map<var_location,T...>::keyed_registry_variant_ptr;

	/**
	 * find a registry
	 */
	keyed_registry_variant_ptr find_registry_variant(const std::string& name){
		return maps.find_registry_variant(name);
	}
	
	/**
	 * find a registry
	 */
	template<typename D>
	keyed_registry<var_location, D>* find_registry(const std::string& name){
		auto ptr = find_registry_variant(name);
		if( ! std::holds_alternative<keyed_registry<var_location, D>*>(ptr) ){
			return nullptr;
		}

		return std::get<keyed_registry<var_location,D>*>(ptr);
	}

	/**
	 * find a var
	 */
	std::variant<T*...> find_variant_variable(const var_descriptor& desc){
		keyed_registry_variant_ptr registry = maps.find_registry_variant(desc.name);

		if(!registry){
			return nullptr;
		}

		return std::visit([loc = desc.location](auto& arg) -> std::variant<T*...> {
			auto val = arg.find(loc);
			return val;
		}, registry);
	}

	/**
	 * find a var if you already know what kind of var it is
	 */
	template<typename D>
	D* find_variable(const var_descriptor& desc){
		keyed_registry_variant* registry = maps.find_registry_variant(desc.name);

		if(!registry){
			return nullptr;
		}
		
		auto& specific_reg = std::get<imb::keyed_registry<imb::var_location, D>>(registry);
		
		return specific_reg.find(desc.location);
	}

	/**
	 * add registry
	 */
	template<typename D>
	bool add_registry(const std::string& key_name){
		return maps.template add_registry<D>(key_name);
	}

	/**
	 * add var
	 * Basically needs a collection builder for this case
	 */
	template<typename D>
	uint64_t add_var(const var_descriptor& desc, const D& value){
		auto find_reg = find_registry_variant(desc.name);
		if(false){

		}

		return 0;
	}

private:
	keyed_registry_map<var_location, T...> maps;
};
}
