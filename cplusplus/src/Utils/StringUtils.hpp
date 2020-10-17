#pragma once
#include <memory>
#include <string>
#include <stdexcept>


// Taken from: https://stackoverflow.com/a/26221725/5181524
template<typename ... Args>
static std::string string_format(const std::string& format, Args ... args) {
    size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
    if(size <= 0) { throw std::runtime_error("Error during formatting."); }
    std::unique_ptr<char[]> buf(new char[size]);
    snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1);
}

template<typename T>
static std::string default_identifier(T* object) {
    return string_format("%s_%x", typeid(T).name(), object);
}