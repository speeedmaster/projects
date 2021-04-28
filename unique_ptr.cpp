#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <utility>

template <typename T, class Deleter = std::default_delete<T>>
class UniquePtr {
private:
    std::tuple<T*, Deleter> a;

public:
    UniquePtr(T* ptr = nullptr) noexcept {
        std::get<0>(a) = ptr;
    }
    UniquePtr(UniquePtr&& other) noexcept {
        std::get<0>(a) = other.release();
        std::get<1>(a) = other.get_deleter();
    }
    UniquePtr(T* ptr, const Deleter& d) {
        std::get<0>(a) = ptr;
        std::get<1>(a) = d;
    }

    UniquePtr& operator=(std::nullptr_t) noexcept {
        std::get<1>(a)(std::get<0>(a));
        std::get<0>(a) = nullptr;
        return *this;
    }
    UniquePtr& operator=(UniquePtr && other) {
        std::swap(a, other.a);
        return *this;
    }
    T& operator*() const {
        return *std::get<0>(a);
    }
    T* operator->() const noexcept {
        return std::get<0>(a);
    }
    T* release() noexcept {
        T* tmp = std::get<0>(a);
        std::get<0>(a) = nullptr;
        return tmp;
    }
    void reset(T* ptr) {
        if (std::get<0>(a) != ptr)
            std::get<1>(a)(std::get<0>(a));
        std::get<0>(a) = ptr;
    }
    void swap(UniquePtr& other) {
        std::swap(a, other.a);
    }
    T* get() const noexcept {
        return std::get<0>(a);
    }
    explicit operator bool() const {
        return std::get<0>(a) != nullptr;
    }
    const Deleter& get_deleter() const {
        return std::get<1>(a);
    }
    ~UniquePtr() {
        std::get<1>(a)(std::get<0>(a));
    }
};
