#pragma once
#include <stdexcept>
#include <utility> // std::move

template<typename T>
class MyVector {
private:
    T* data;
    size_t capacity;
    size_t length;

    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < length; ++i) {
            newData[i] = std::move(data[i]); // move semantics
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    MyVector() : data(nullptr), capacity(0), length(0) {
        resize(4);
    }

    // Copy constructor
    MyVector(const MyVector& other) : data(nullptr), capacity(0), length(0) {
        resize(other.length);
        for (size_t i = 0; i < other.length; ++i) {
            data[i] = other.data[i];
        }
        length = other.length;
    }

    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            data = nullptr;
            capacity = 0;
            length = 0;
            resize(other.length);
            for (size_t i = 0; i < other.length; ++i) {
                data[i] = other.data[i];
            }
            length = other.length;
        }
        return *this;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            capacity = other.capacity;
            length = other.length;
            other.data = nullptr;
            other.capacity = 0;
            other.length = 0;
        }
        return *this;
    }

    ~MyVector() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (length >= capacity) {
            resize(capacity == 0 ? 4 : capacity * 2);
        }
        data[length++] = value;
    }

    void pop_back() {
        if (length > 0) {
            --length;
        }
    }

    void erase(size_t index) {
        if (index >= length) throw std::out_of_range("Index out of range");
        for (size_t i = index; i < length - 1; ++i) {
            data[i] = std::move(data[i + 1]);
        }
        --length;
    }

    T& operator[](size_t index) {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= length) throw std::out_of_range("Index out of range");
        return data[index];
    }

    T& back() {
        if (length == 0) throw std::out_of_range("Vector kosong");
        return data[length - 1];
    }

    size_t size() const {
        return length;
    }

    bool empty() const {
        return length == 0;
    }

    void clear() {
        length = 0;
    }
};
