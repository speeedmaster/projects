#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <cassert>
#include <iostream>
#include <memory>

struct Node {
    Node* left;
    Node* right;
    virtual ~Node() = default;
};

template <class T>
struct ListNode : public Node {
    ListNode() = default;
    ListNode(T& value) : value(value) {}
    ListNode(T && value) : value(std::move(value)) {}
    T value;
};

template <class T>
class List {
private:
    static void bind(Node* node, Node* where) {
        auto right = where->right;
        right->left = node;
        where->right = node;
        node->right = right;
        node->left = where;
    }

    static void unbind(Node* node) {
        auto left = node->left;
        auto right = node->right;
        left->right = right;
        right->left = left;
        delete node;
    }

    Node* cap;
    size_t size_ = 0;

public:
    List() : cap(new Node()) {
        cap->left = cap;
        cap->right = cap;
    }
    List(const List& other) : cap(new Node()) {
        cap->left = cap;
        cap->right = cap;
        for (const T& elem : other) {
            (*this).push_back(elem);
        }
    }

    List& operator=(const List& other) {
        while (size_ != 0) {
            (*this).pop_front();
        }
        for (const T& elem : other) {
            (*this).push_back(elem);
        }
        return *this;
    }
    ~List() {
        auto cur = cap->right;
        while (cur != cap) {
            auto next = cur->right;
            delete cur;
            cur = next;
        }
        delete cap;
    }

    void push_back(const T& value) {
        try {
            auto node = new ListNode(value);
            bind(node, cap->left);
            ++size_;
        } catch (...) {
            throw;
        }
    }

    void push_front(const T& value) {
        try {
            auto node = new ListNode(value);
            bind(node, cap);
            ++size_;
        } catch (...) {
            throw;
        }
    }

    void pop_back() {
        unbind(cap->left);
        --size_;
    }

    void pop_front() {
        unbind(cap->right);
        --size_;
    }

    size_t size() const {
        return size_;
    }

    class Iterator : public std::iterator<std::random_access_iterator_tag, T>{
    private:
        Node* pointer_;

    public:
        Iterator(Node* pointer) : pointer_(pointer) {}
        Iterator(const Iterator& x) : pointer_(x.pointer_) {}
        Iterator(Iterator&& x) : pointer_(std::move(x.pointer_)) {}
        Iterator& operator++() {
            pointer_ = pointer_->right;
            return *this;
        }
        Iterator& operator++(int) {
            Iterator now(*this);
            pointer_ = pointer_->right;
            return *this;
        }
        Iterator& operator--() {
            pointer_ = pointer_->left;
            return *this;
        }
        Iterator& operator--(int) {
            Iterator now(*this);
            pointer_ = pointer_->left;
            return *this;
        }
        T& operator*() const {
            return static_cast<ListNode<T>*>(pointer_)->value;
        }
        T* operator->() const {
            return *(static_cast<ListNode<T>*>(pointer_)->value);
        }
        bool operator==(const Iterator& x) const {
            return pointer_ == x.pointer_;
        }
        bool operator!=(const Iterator& x) const {
            return pointer_ != x.pointer_;
        }
    };
    Iterator begin() {
        return Iterator(cap->right);
    }
    Iterator end() {
        return Iterator(cap);
    }
};
