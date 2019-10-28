#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <string>
#include <iostream>
#include <random>
#include <functional>
#include <cmath>
#include <algorithm>
#include <chrono>

#include "lru.h"

template<typename TKey, typename TValue>
using LRUMapList = lpq::LRU<TKey, TValue, std::map<TKey, TValue>, std::list<TKey>>;

template<typename TKey, typename TValue>
using LRUMapVector = lpq::LRU<TKey, TValue, std::map<TKey, TValue>, std::vector<TKey>>;

template<typename TKey, typename TValue>
using LRUHashList = lpq::LRU<TKey, TValue, std::unordered_map<TKey, TValue>, std::list<TKey>>;

template<typename TKey, typename TValue>
using LRUHashVector = lpq::LRU<TKey, TValue, std::unordered_map<TKey, TValue>, std::vector<TKey>>;

std::default_random_engine engine;
std::uniform_int_distribution<int> distribution(0, 64);
std::string RandomString(size_t length) {
	std::string random_str(length, '\0');
	for (size_t i = 0; i < length; ++i) {
		auto ch = distribution(engine);
		if (ch < 26) {
			random_str[i] = 'A' + ch;
		}
		else if (ch < 52) {
			random_str[i] = 'a' + (ch - 26);
		}
		else if (ch < 62) {
			random_str[i] = '0' + (ch - 52);
		}
		else {
			random_str[i] = ch == 62 ? '-' : '_';
		}
	}
	return random_str;
}

#define M_PI       3.14159265358979323846   // pi
std::function<double(double)> Normal(double sigma, double factor) {
	double a = sigma * std::sqrt(2 * M_PI);
	a = 1 / a;
	double b = -1 / (2 * sigma * sigma);
	return [=](double x) -> double {
		return a * exp(b * x * x) * factor;
	};
}

std::vector<std::string> GenerateVector(size_t length, double sigma){
	std::vector<std::string> result;
	result.reserve(length);
	auto normal = Normal(sigma, static_cast<double>(length));
	const size_t STR_LEN = 16;
	for (int i = 0; i < 4 * sigma; ++i) {
		auto len = static_cast<size_t>(std::round(normal(i)));
		if (len == 0) {
			break;
		}
		auto str = RandomString(STR_LEN);
		std::generate_n(std::back_inserter(result), len, [&]() {return str; });
	}
	for (size_t i = result.size(); i < length; ++i) {
		result.push_back(RandomString(STR_LEN));
	}
	std::random_shuffle(result.begin(), result.end());
	std::random_shuffle(result.begin(), result.end());
	return result;
}

template<typename LRUType, typename ValuesType>
size_t TestLRU(const ValuesType& values) {
	LRUType lru(values.size());
	size_t hit = 0;
	size_t sum = 0;
	auto start = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < values.size(); ++i) {
		const auto& v = values[i];
		size_t index = -1;
		if (lru.CheckOut(v, index)) {
			++hit;
		}
		else {
			index = i;
			lru.CheckIn(v, i);
		}
		sum += index;
	}
	auto finish = std::chrono::high_resolution_clock::now();
	std::cout << "cost:" << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count()
		<< "ms\thit rate:" << hit * 100.0 / values.size() << "%" << std::endl;
	return hit;
}

int main() {
	auto v1 = GenerateVector(8192, 1);
	std::cout << "LRUMapVector\t";
	TestLRU < LRUMapVector <std::string, size_t>>(v1);
	std::cout << "LRUMapList\t";
	TestLRU < LRUMapList<std::string, size_t>>(v1);
	std::cout << "LRUHashVector\t";
	TestLRU < LRUHashVector<std::string, size_t>>(v1);
	std::cout << "LRUHashList\t";
	TestLRU < LRUHashList<std::string, size_t>>(v1);
	std::cout << std::endl;

	v1 = GenerateVector(8192, 8);
	std::cout << "LRUMapVector\t";
	TestLRU < LRUMapVector <std::string, size_t>>(v1);
	std::cout << "LRUMapList\t";
	TestLRU < LRUMapList<std::string, size_t>>(v1);
	std::cout << "LRUHashVector\t";
	TestLRU < LRUHashVector<std::string, size_t>>(v1);
	std::cout << "LRUHashList\t";
	TestLRU < LRUHashList<std::string, size_t>>(v1);
	std::cout << std::endl;

	v1 = GenerateVector(8192, 256);
	std::cout << "LRUMapVector\t";
	TestLRU < LRUMapVector <std::string, size_t>>(v1);
	std::cout << "LRUMapList\t";
	TestLRU < LRUMapList<std::string, size_t>>(v1);
	std::cout << "LRUHashVector\t";
	TestLRU < LRUHashVector<std::string, size_t>>(v1);
	std::cout << "LRUHashList\t";
	TestLRU < LRUHashList<std::string, size_t>>(v1);
	std::cout << std::endl;

	v1 = GenerateVector(8192, 2048);
	std::cout << "LRUMapVector\t";
	TestLRU < LRUMapVector <std::string, size_t>>(v1);
	std::cout << "LRUMapList\t";
	TestLRU < LRUMapList<std::string, size_t>>(v1);
	std::cout << "LRUHashVector\t";
	TestLRU < LRUHashVector<std::string, size_t>>(v1);
	std::cout << "LRUHashList\t";
	TestLRU < LRUHashList<std::string, size_t>>(v1);
	std::cout << std::endl;

	v1 = GenerateVector(8192, 2100);
	std::cout << "LRUMapVector\t";
	TestLRU < LRUMapVector <std::string, size_t>>(v1);
	std::cout << "LRUMapList\t";
	TestLRU < LRUMapList<std::string, size_t>>(v1);
	std::cout << "LRUHashVector\t";
	TestLRU < LRUHashVector<std::string, size_t>>(v1);
	std::cout << "LRUHashList\t";
	TestLRU < LRUHashList<std::string, size_t>>(v1);
	std::cout << std::endl;
	return 0;
}