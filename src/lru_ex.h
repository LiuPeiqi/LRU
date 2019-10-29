#include <utility>
#include <queue>
namespace lpq {
	template <typename TKey, typename TValue,
		template <typename, typename> class TMap>
	class LRUEx {
	public:
		LRUEx(size_t cache_capacity, size_t cache_reorder_length = 0)
			:capacity(cache_capacity), frame(0), reorder_length(cache_reorder_length) {}

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
			if (cache_size < (capacity + reorder_length)) {
				return;
			}
			auto comp = [](const TKeyPair& left, const TKeyPair& right) {
				return left.second < right.second;
			};
			std::priority_queue<TKeyPair, std::vector<TKeyPair>, decltype(comp)> remove_list(comp);
			auto remove_length = reorder_length == 0 ? 1 : reorder_length;
			for (auto iter = cache_map.begin(); iter != cache_map.end(); ++iter) {
				auto key = iter->first;
				auto this_frame = iter->second.second;
				if (remove_list.size() < remove_length) {
					remove_list.push(std::make_pair(key, this_frame));
				}
				else if (remove_list.top().second <= this_frame) {
					continue;
				}
				else {
					remove_list.pop();
					remove_list.push(std::make_pair(key, this_frame));
				}
			}
			while (!remove_list.empty()) {
				auto item = remove_list.top();
				Remove(item.first);
				remove_list.pop();
			}
		}

		bool Remove(const TKey& key) {
			auto iter = cache_map.find(key);
			if (iter == cache_map.end()) 
			{
				return false;
			}
			cache_map.erase(iter);
			return true;
		}

	private:
		using TKeyPair = std::pair<TKey, size_t>;

		TMap<TKey, std::pair<TValue, size_t>> cache_map;
		size_t capacity;
		size_t reorder_length;
		size_t frame;
	};
}  // namespace lpq
