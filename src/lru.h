#include <algorithm>
#include <memory>

namespace lpq {
template <typename TKey, typename TValue, 
	template <typename, typename> class TMap, 
	template <typename> class TContainer>
class LRU {
public:
    LRU(size_t cache_capacity) : capacity(cache_capacity) {}

    bool CheckOut(const TKey& key, TValue& value) {
        auto iter = cache_map.find(key);
        if (iter == cache_map.end()) { return false; }
        value = iter->second;
        auto list_item = std::find(cache_list.begin(), cache_list.end(), key);
        cache_list.erase(list_item);
        cache_list.push_back(key);
        return true;
    }

    void CheckIn(const TKey& key, const TValue& value) {
        if (cache_list.size() == capacity) {
            auto past = cache_list.front();
            cache_list.erase(cache_list.begin());
            cache_map.erase(past);
        }
        cache_map[key] = value;
        cache_list.push_back(key);
    }

    bool Remove(const TKey& key) {
        auto iter = cache_map.find(key);
        if (iter == cache_map.end()) { return false; }
        cache_map.remove(iter);
        cache_list.earse(iter.first);
        return true;
    }

private:
    TMap<TKey, TValue> cache_map;
    TContainer<TKey> cache_list;
    size_t capacity;
};
}  // namespace lpq
