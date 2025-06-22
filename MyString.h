#ifndef CUSTOM_STRING_HPP
#define CUSTOM_STRING_HPP

#include <cstring>
#include <iostream>

class MyString {
private:
    char* data;
    size_t len;

public:
    MyString() : data(new char[1]), len(0) {
    data[0] = '\0';
    }
    
    MyString(const char* s) {
        len = std::strlen(s);
        data = new char[len + 1];
        std::strcpy(data, s);
    }

    MyString(const std::string& s) {
        len = s.length();
        data = new char[len + 1];
        std::strcpy(data, s.c_str());
    }

    MyString(const MyString& other) {
        len = other.len;
        data = new char[len + 1];
        std::strcpy(data, other.data);
    }

    MyString& operator=(const MyString& other) {
        if (this != &other) {
            delete[] data;
            len = other.len;
            data = new char[len + 1];
            std::strcpy(data, other.data);
        }
        return *this;
    }

    MyString& operator=(const std::string& s) {
        delete[] data;
        len = s.length();
        data = new char[len + 1];
        std::strcpy(data, s.c_str());
        return *this;
    }

    ~MyString() {
        delete[] data;
    }

    size_t size() const {
        return len;
    }

    size_t length() const {
        return size();
    }

    char& operator[](size_t i) {
        return data[i];
    }

    const char& operator[](size_t i) const {
        return data[i];
    }

    const char* c_str() const {
        return data;
    }
    
    std::string toStdString() const {
        return std::string(data);
    }

    friend std::ostream& operator<<(std::ostream& os, const MyString& s) {
        return os << s.data;
    }

    friend std::istream& operator>>(std::istream& is, MyString& s) {
        char buffer[1024];
        is.getline(buffer, 1024);
        s = MyString(buffer);
        return is;
    }

    bool operator==(const MyString& other) const {
        return std::strcmp(data, other.data) == 0;
    }

    MyString operator+(const MyString& other) const {
        char* buffer = new char[len + other.len + 1];
        std::strcpy(buffer, data);
        std::strcat(buffer, other.data);
        MyString result(buffer);
        delete[] buffer;
        return result;
    }
};

#endif
