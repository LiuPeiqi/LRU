#include <functional>
#include <queue>
#include <utility>
namespace lpq {
template <typename TKey, typename TValue,
          template <typename, typename> class TMap>
class LRUEx {
public:
    LRUEx(size_t cache_capacity, size_t cache_reorder_length = 0)
        : capacity(cache_capacity),
          frame(0),
          remove_list(TKeyPairComp),
          reorder_length(cache_reorder_length) {
        if (reorder_length < 1) { reorder_length = 1; }
    }

    bool CheckOut(const TKey& key, TValue& value) {
        auto iter = cache_map.find(key);
        if (iter == cache_map.end()) { return false; }
        value = iter->second.first;
        ++frame;
        iter->second.second = frame;
        return true;
    }

    void CheckIn(const TKey& key, const TValue& value) {
        ++frame;
        cache_map[key] = std::make_pair(value, frame);
        auto cache_size = cache_map.size();
        if (cache_size < (capacity + reorder_length)) { return; }
        for (auto iter = cache_map.begin(); iter != cache_map.end(); ++iter) {
            const auto* key = &iter->first;
            auto this_frame = iter->second.second;
            if (remove_list.size() < reorder_length) {
                remove_list.push(std::make_pair(key, this_frame));
            } else if (remove_list.top().second <= this_frame) {
                continue;
            } else {
                remove_list.pop();
                remove_list.push(std::make_pair(key, this_frame));
            }
        }
        while (!remove_list.empty()) {
            auto item = remove_list.top();
            Remove(*item.first);
            remove_list.pop();
        }
    }

    bool Remove(const TKey& key) {
        auto iter = cache_map.find(key);
        if (iter == cache_map.end()) { return false; }
        cache_map.erase(iter);
        return true;
    }

private:
    using TKeyPair = std::pair<const TKey*, size_t>;
    const std::function<bool(const TKeyPair&, const TKeyPair&)>
        TKeyPairComp = [](const TKeyPair& left, const TKeyPair& right) -> bool {
        return left.second < right.second;
    };

    TMap<TKey, std::pair<TValue, size_t>> cache_map;
    std::priority_queue<TKeyPair, std::vector<TKeyPair>, decltype(TKeyPairComp)>
        remove_list;
    size_t capacity;
    size_t reorder_length;
    size_t frame;
};
}  // namespace lpq
