#include <algorithm>
#include <sstream>
#include <stdexcept>
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
    size_type queue_length;
    size_type cursor_index;

public:
    circle_queue() : container(), queue_length(0), cursor_index(0) {}
    circle_queue(const circle_queue& other)
        : container(other.container),
          queue_length(other.queue_length),
          cursor_index(0) {}
    circle_queue& operator=(const circle_queue& other) {
        container = other.container;
        queue_length = other.queue_length;
        cursor_index = other.cursor_index;
        return *this;
    }
    circle_queue(circle_queue&& other)
        : container(std::move(other.container)),
          queue_length(other.queue_length),
          cursor_index(other.cursor_index) {}
    circle_queue& operator=(circle_queue&& other) {
        container = std::move(other.container);
        queue_length = other.queue_length;
        cursor_index = other.cursor_index;
        return *this;
    }

    bool empty() const { return queue_length == 0; }
    size_type size() const { return queue_length; }
    void push(const value_type& value) {
        auto container_size = container.size();
        if (queue_length >= container_size) { expand(1); }
        container_size = container.size();
        auto circle_end = cursor_index + queue_length;
        auto is_wrap = circle_end >= container_size;
        if (is_wrap) { circle_end -= container_size; }
        container[circle_end] = value;
        ++queue_length;
    }
    void pop() {
        if (queue_length == 0) { return; }
        ++cursor_index;
        --queue_length;
        auto container_size = container.size();
        if (cursor_index >= container_size) { cursor_index = 0; }
    }
    const_reference top() const {
        auto top_index = cursor_index + queue_length - 1;
        auto container_size = container.size();
        if (top_index >= container_size) { top_index -= container_size; }
        return container_size[top_index];
    }

    void expand(size_type count) {
        if (count == 0) { return; }
        auto old_size = container.size();
        auto circle_end = cursor_index + queue_length;
        container.resize(old_size + count);
        if (circle_end <= old_size) { return; }
        circle_end -= old_size;
        auto old_end = container.begin() + old_size;
        auto move_length = std::min(circle_end, count);
        auto first = container.begin();
        auto finish = first + move_length;
        std::move(first, finish, old_end);
        if (move_length < circle_end) {
            std::move(finish, first + circle_end, first);
        }
    }
    size_type trim() {
        auto container_size = container.size();
        auto circle_end = cursor_index + queue_length;
        container_type trimed_container;
        trimed_container.reserve(queue_length);
        auto trimed_count = container.size() - queue_length;
        if (circle_end <= container_size) {
            auto start = container.begin() + cursor_index;
            auto finish = start + queue_length;
            std::move(start, finish, std::back_inserter(trimed_container));
        } else {
            auto start = container.begin() + cursor_index;
            std::move(start, container.end(),
                      std::back_inserter(trimed_container));
            auto finish = container.begin() + circle_end - container_size;
            std::move(container.begin(), finish,
                      std::back_inserter(trimed_container));
        }
        std::swap(trimed_container, container);
        cursor_index = 0;
        return trimed_count;
    }
    const_reference operator[](size_type index) {
        if (index >= queue_length) {
            std::stringstream sst;
            sst << "index circle_queue out_of_range, index:" << index << "\t"
                << "queue length:" << queue_length << "!";
            throw std::out_of_range(sst.str());
        }
        auto cursor_end = cursor_index + index;
        auto container_size = container.size();
        auto is_wrap = cursor_end >= container_size;
        if (is_wrap) { cursor_end -= container_size; }
        return container[cursor_end];
    }
};
}  // namespace lpq
