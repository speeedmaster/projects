#include <iostream>
#include <memory>

template <typename T>
class Vector {
private:
    T* data = nullptr;
    size_t size_ = 0, capacity_ = 0;

    T* Allocate(size_t n) {
        return reinterpret_cast<T*>(operator new(n * sizeof(T)));
    }

    void Deallocate(T* ptr) {
        operator delete(ptr);
    }

public:
    Vector(size_t size = 0) : data(Allocate(size)), size_(size), capacity_(size) {
        try {
            std::uninitialized_value_construct_n(data, size);
        } catch (...) {
            Deallocate(data);
            throw;
        }
    }

    Vector(const Vector& other) : size_(other.size_), capacity_(other.size_) {
        if (size_ == 0) {
            return;
        }
        data = Allocate(other.size_);
        try {
            std::uninitialized_copy_n(other.data, other.size_, data);
        } catch (...) {
            Deallocate(data);
            throw;
        }
    }

    Vector& operator=(Vector other) {
        std::swap(data, other.data);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        return *this;
    }

    void reserve(size_t size) {
        if (capacity_ < size) {
            T* new_data = Allocate(size);
            try {
                std::uninitialized_copy_n(data, size_, new_data);
                std::destroy_n(data, size_);
                Deallocate(data);
                data = new_data;
                capacity_ = size;
            } catch (...) {
                Deallocate(new_data);
                throw;
            }
        }
    }

    void resize(size_t size) {
        reserve(size);
        if (size_ < size) {
            std::uninitialized_value_construct(data + size_, data + size);
            size_ = size;
        } else if (size_ > size) {
            std::destroy(data + size, data + size_);
            size_ = size;
        }
    }

    void clear() {
        std::destroy(data, data + size_);
        size_ = 0;
    }

    void push_back(const T& x) {
        if (size_ == capacity_) {
            if (capacity_ == 0)
                reserve(1);
            else
                reserve(capacity_ * 2);
        }
        new(data + size_) T(x);
        ++size_;
    }

    void push_back(T&& x) {
        if (size_ == capacity_) {
            if (capacity_ == 0)
                reserve(1);
            else
                reserve(capacity_ * 2);
        }
        new(data + size_) T(std::move(x));
        ++size_;
    }

    void pop_back() {
        --size_;
        (data + size_)->~T();
    }

    T* begin() {
        return data;
    }

    const T* begin() const {
        return data;
    }

    T* end() {
        return data + size_;
    }

    const T* end() const {
        return data + size_;
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    const T& operator[](size_t i) const {
        return data[i];
    }

    T& operator[](size_t i) {
        return data[i];
    }

    void swap(Vector& other) {
        std::swap(data, other.data);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
    }

    ~Vector() {
        std::destroy_n(data, size_);
        operator delete(data);
    }
};
