#pragma once

namespace imb {

template<typename Schema, typename Storage>
class Variable {
public:
	Variable(typename Schema::Type value):storage{value}{}


private:
	Storage<Schema> storage;
};


}
