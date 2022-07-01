#pragma once

#include "key_registry.h"
#include "var_descriptor.h"

#include <variant>
#include <optional>

namespace imb {

template<typename... T>
class descriptor_registry_map {
public:
	using keyed_registry_variant = typename keyed_registry_map<var_location,T...>::keyed_registry_variant;

	/**
	 *
	 */
	keyed_registry_variant* find_registry_variant(const std::string& name){
		return maps.find_registry_variant(name);
	}

	/**
	 * 
	 */
	std::variant<T*...> find_variant_variable(const var_descriptor& desc){
		keyed_registry_variant* registry = maps.find_registry_variant(desc.name);

		if(!registry){
			return std::nullopt;
		}

		return std::visit([loc = desc.location](auto& arg) -> std::variant<T*...> {
			auto* val = arg.find(loc);
			return val;
		}, *registry);
	}

	/**
	 *
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

private:
	keyed_registry_map<var_location, T...> maps;
};
}
