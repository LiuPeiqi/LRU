#include <algorithm>
#include <chrono>
#include <cmath>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "lru.h"
#include "lru_ex.h"

template <typename TKey, typename TValue>
using std_map = std::map<TKey, TValue>;

template <typename TKey, typename TValue>
using std_unordered_map = std::unordered_map<TKey, TValue>;

template <typename TKey>
using std_list = std::list<TKey>;

template <typename TKey>
using std_vector = std::vector<TKey>;

template <typename TKey, typename TValue>
using LRUMapList = lpq::LRU<TKey, TValue, std_map, std_list>;

template <typename TKey, typename TValue>
using LRUMapVector = lpq::LRU<TKey, TValue, std_map, std_vector>;

template <typename TKey, typename TValue>
using LRUHashList = lpq::LRU<TKey, TValue, std_unordered_map, std_list>;

template <typename TKey, typename TValue>
using LRUHashVector = lpq::LRU<TKey, TValue, std_unordered_map, std_vector>;

template <typename TKey, typename TValue>
using LRUExMapVector = lpq::LRUEx<TKey, TValue, std_map>;

std::default_random_engine engine;
std::uniform_int_distribution<int> distribution(0, 64);
std::string RandomString(size_t length) {
    std::string random_str(length, '\0');
    for (size_t i = 0; i < length; ++i) {
        auto ch = distribution(engine);
        if (ch < 26) {
            random_str[i] = 'A' + ch;
        } else if (ch < 52) {
            random_str[i] = 'a' + (ch - 26);
        } else if (ch < 62) {
            random_str[i] = '0' + (ch - 52);
        } else {
            random_str[i] = ch == 62 ? '-' : '_';
        }
    }
    return random_str;
}

#define M_PI 3.14159265358979323846  // pi
std::function<double(double)> Normal(double sigma, double factor) {
    double a = sigma * std::sqrt(2 * M_PI);
    a = 1 / a;
    double b = -1 / (2 * sigma * sigma);
    return [=](double x) -> double { return a * exp(b * x * x) * factor; };
}

std::vector<std::string> GenerateVector(size_t length, double sigma) {
    std::vector<std::string> result;
    result.reserve(length);
    auto normal = Normal(sigma, static_cast<double>(length));
    const size_t STR_LEN = 16;
    for (int i = 0; i < 4 * sigma; ++i) {
        auto len = static_cast<size_t>(std::round(normal(i)));
        if (len == 0) { break; }
        auto str = RandomString(STR_LEN);
        std::generate_n(std::back_inserter(result), len, [&]() { return str; });
    }
    for (size_t i = result.size(); i < length; ++i) {
        result.push_back(RandomString(STR_LEN));
    }
    std::random_shuffle(result.begin(), result.end());
    std::random_shuffle(result.begin(), result.end());
    return result;
}

template <typename KeysType, typename ValuesType, typename LRUType>
size_t TestLRU(const KeysType& keys, const ValuesType& values, LRUType&& lru) {
    size_t hit = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < values.size(); ++i) {
        const auto& k = keys[i];
        const auto& v = values[i];
        std::string cache_value;
        if (lru.CheckOut(k, cache_value)) {
            ++hit;
        } else {
            lru.CheckIn(k, v);
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << "cost:"
              << std::chrono::duration_cast<std::chrono::milliseconds>(finish -
                                                                       start)
                     .count()
              << "ms\thit rate:" << hit * 100.0 / values.size() << "%"
              << std::endl;
    return hit;
}

const size_t VECTOR_SIZE = 1024 * 10;
const size_t LRU_CAPACITY = 1024;
const size_t sigmas[] = {1, 8, 256, 2048, 2600};
int main() {
    for (auto sigma : sigmas) {
        auto v1 = GenerateVector(VECTOR_SIZE, static_cast<double>(sigma));
        auto key1 = std::vector<size_t>();
        std::transform(v1.begin(), v1.end(), std::back_inserter(key1),
                       std::hash<std::string>{});
        std::cout << "LRUMapVector\t";
        TestLRU(key1, v1, LRUMapVector<size_t, std::string>(LRU_CAPACITY));
        std::cout << "LRUMapList\t";
        TestLRU(key1, v1, LRUMapList<size_t, std::string>(LRU_CAPACITY));
        std::cout << "LRUHashVector\t";
        TestLRU(key1, v1, LRUHashVector<size_t, std::string>(LRU_CAPACITY));
        std::cout << "LRUHashList\t";
        TestLRU(key1, v1, LRUHashList<size_t, std::string>(LRU_CAPACITY));
        std::cout << "LRUExMapVector\t";
        TestLRU(key1, v1,
                LRUExMapVector<size_t, std::string>(LRU_CAPACITY - 256, 256));
        std::cout << std::endl;
    }
    return 0;
}
