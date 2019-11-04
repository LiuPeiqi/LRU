#include <utility>
#include <vector>

namespace lpq {
template <typename TValue, class Container = std::vector<TValue>>
class circle_queue {
public:
    typedef Container container_type;
    typedef typename container_type::value_type value_type;
    typedef typename container_type::reference reference;
    typedef typename container_type::const_reference const_reference;
    typedef typename container_type::size_type size_type;
    static_assert((std::is_same<TValue, value_type>::value), "");

protected:
    container_type container;
    size_type max_queue_length;
    size_type cursor_index;

public:
    circle_queue(size_type max_length)
        : container(), max_queue_length(max_length), cursor_index(0) {}
    circle_queue(const circle_queue& other)
        : container(other.container),
          max_queue_length(other.max_queue_length),
          cursor_index(0) {}
    circle_queue& operator=(const circle_queue& other) {
        container = other.container;
        max_queue_length = other.max_queue_length;
        cursor_index = other.cursor_index;
        return *this;
    }
    circle_queue(circle_queue&& other)
        : container(std::move(other.container)),
          max_queue_length(other.max_queue_length),
          cursor_index(other.cursor_index) {}
    circle_queue& operator=(circle_queue&& other) {
        container = std::move(other.container);
        max_queue_length = other.max_queue_length;
        cursor_index = other.cursor_index;
        return *this;
    }
};
}  // namespace lpq
