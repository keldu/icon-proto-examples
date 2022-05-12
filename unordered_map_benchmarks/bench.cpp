#include <cassert>
#include <cstdint>

#include <chrono>
#include <iomanip>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <vector>

namespace ben {
/**
 * Basically same as an std::unordered_map
 */
template<typename K, typename V>
class SimpleUnordered {
public:
	std::unordered_map<K, V> data;
public:
	bool insert(const K& key, V&& value){
		return data.insert(std::make_pair(key, std::move(value))).second;
	}

	V* get(const K& key){
		auto find = data.find(key);
		return find != data.end() ? &find->second : nullptr;
	}
};

/**
 * Unordered map with an exposed interface for accessing the ids fast
 * The basic data is stored in a vector while the more complex keys are stored in an unordered_map
 * The concept is similar to how a sql database works
 *
 * This method has some disadvantages
 * You are always assigned an id. You don't choose it yourself
 */
template<typename K, typename V>
class IndexedUnordered {
public:
	std::vector<V> data;

	std::unordered_map<K, uint64_t> index;

public:
	std::optional<uint64_t> insert(const K& key, V&& value){
		auto id = data.size();
		data.push_back(std::move(value));

		auto insert_op = index.insert(std::make_pair(key, id));

		if(!insert_op.second){
			data.pop_back();
			return std::nullopt;
		}

		return id;
	}

	V* get(const K& k){
		auto find = index.find(k);
		if(find == index.end()){
			return nullptr;
		}

		uint64_t id = find->second;
		return index_get(id);
	}

	V* index_get(uint64_t id){
		assert(id < data.size());
		if(id >= data.size()){
			return nullptr;
		}

		return &data.at(id);
	}
};

/**
 * Key used for acces in the memory manager
 */
struct ExampleKey {
	uint64_t a;
	uint32_t b;
	int64_t c;

	bool operator==(const ExampleKey& k) const {
		return (a == k.a && b == k.b && c == k.c);
	}
};

/**
 * Just so a Value exists
 */
struct ExampleValue {
	uint16_t value;
};

/**
 * Helpers for the range iteration
 */
struct ExampleKeyRange{
	ExampleKey lower{
		0, 0, -50
	};

	ExampleKey upper{
		100, 100, 50
	};

	void increment(ExampleKey& k){
		++k.a;

		if(k.a > upper.a){
			k.a = lower.a;
			++k.b;

			if(k.b > upper.b){
				k.b = lower.b;
				++k.c;

				if(k.c > upper.c){
					k.c = lower.c;
				}
			}
		}
	}
};

/**
 * Just the instantiation of the range
 */
static ExampleKeyRange range;

}

/**
 * Custom hashing. Not the best. But it'll do
 */
namespace std {
template<>
struct hash<ben::ExampleKey> {
	std::size_t operator()(const ben::ExampleKey& k) const {
		using std::hash;

		return ((hash<uint64_t>()(k.a)
			^ (hash<uint32_t>()(k.b) << 1)) >> 1)
			^ (hash<int64_t>()(k.c) << 1);
	}
};
}
int main(){

	auto duration_simple = [](){
		ben::SimpleUnordered<ben::ExampleKey, ben::ExampleValue> map;

		for(ben::ExampleKey key = ben::range.lower; !(key == ben::range.upper); ben::range.increment(key)){
			map.insert(key, ben::ExampleValue{});
		}
		auto start = std::chrono::steady_clock::now();
		for(ben::ExampleKey key = ben::range.lower; !(key == ben::range.upper); ben::range.increment(key)){
			auto unused_stuff = map.get(key);
			(void) unused_stuff;
		}
		auto stop = std::chrono::steady_clock::now();
		return stop - start;
	}();

	auto duration_indexed = [](){
		ben::IndexedUnordered<ben::ExampleKey, ben::ExampleValue> map;
		std::vector<uint64_t> ids;
		for(ben::ExampleKey key = ben::range.lower; !(key == ben::range.upper); ben::range.increment(key)){
			auto opt_id = map.insert(key, ben::ExampleValue{});
			if(opt_id.has_value()){
				ids.push_back(opt_id.value());
			}
		}
		auto start = std::chrono::steady_clock::now();
		for(ben::ExampleKey key = ben::range.lower; !(key == ben::range.upper); ben::range.increment(key)){
			auto unused_stuff = map.get(key);
			(void) unused_stuff;
		}
		auto stop = std::chrono::steady_clock::now();
		auto start_fast = std::chrono::steady_clock::now();
		for(uint64_t id : ids){
			auto unused_stuff = map.index_get(id);
			(void) unused_stuff;
		}
		auto stop_fast = std::chrono::steady_clock::now();

		return std::make_pair(stop - start, stop_fast - start_fast);
	}();

	auto dur_simple_count = duration_simple.count();
	auto dur_index_count = duration_indexed.first.count();
	auto dur_index_fast_count = duration_indexed.second.count();

	double base_simple = static_cast<double>(dur_simple_count) / 100.0;
	double perc_index = dur_index_count / base_simple;
	double perc_index_fast = dur_index_fast_count / base_simple;

	std::cout<<"Time in ns\n"<<std::setw(16)<<dur_simple_count<<std::setw(16)<<dur_index_count<<std::setw(16)<<dur_index_fast_count<<'\n';
	std::cout<<std::setw(16)<<"100%"<<std::setw(15)<<perc_index<<std::setw(1)<<"%"<<std::setw(15)<<perc_index_fast<<std::setw(1)<<'%'<<std::endl;

	return 0;
}
