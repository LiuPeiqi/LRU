#include <list>
#include <utility>
namespace lpq {
template <typename TKey, typename TValue,
          template <typename, typename> class TMap>
class LRUEx2 {
public:
    LRUEx2(size_t cache_capacity) : capacity(cache_capacity) {}
    bool CheckOut(const TKey& key, TValue& value) {
        auto iter = cache_map.find(key);
        if (iter == cache_map.end()) { return false; }
        auto& node = iter->second;
        value = node.second;
        cache_list.erase(node.first);
        cache_list.push_front(key);
        node.first = cache_list.begin();
        return true;
    }

    void CheckIn(const TKey& key, const TValue& value) {
        auto iter = cache_map.find(key);
        if (iter == cache_map.end()) {
            if (cache_map.size() == capacity) {
                const auto& last_key = cache_list.back();
                auto last_iter = cache_map.find(last_key);
                cache_map.erase(last_iter);
                cache_list.pop_back();
            }
            cache_list.push_front(key);
            cache_map[key] = std::make_pair(cache_list.begin(), value);
        } else {
            auto& node = iter->second;
            node.second = value;
            cache_list.erase(node.first);
            cache_list.push_front(key);
            node.first = cache_list.begin();
        }
    }

    bool Remove(const TKey& key) {
        auto iter = cache_map.find(key);
        if (iter == cache_map.end()) { return false; }
        cache_list.erase(iter->second.first);
        cache_map.erase(iter);
        return true;
    }

private:
    using TNode = std::pair<typename std::list<TKey>::iterator, TValue>;
    TMap<TKey, TNode> cache_map;
    std::list<TKey> cache_list;
    size_t capacity;
};
}  // namespace lpq
