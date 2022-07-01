#pragma once

namespace imb {
namespace schema {

template<typename T, size_t N>
struct Array {
	using Type = typename T::Type;
};

struct Float32 {
	using Type = float;
};

struct Float64 {
	using Type = double;
};

}
}
